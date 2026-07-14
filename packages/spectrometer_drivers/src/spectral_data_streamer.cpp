#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>

#include "ftd2xx.h"
#include "libft4222.h"

#include "rclcpp/rclcpp.hpp"
#include "spectrometer_interfaces/msg/spectra.hpp"
#include "spectrometer_interfaces/srv/integration.hpp"

// IbsenLinuxExample.cpp modified by Gary Lvov and Nathaniel Hanson
// ROS2 (rclcpp) port: ROS layer updated, FT4222 SPI hardware logic preserved.

using namespace std::chrono_literals;

class IbsenDriver : public rclcpp::Node
{
public:
    IbsenDriver()
    : rclcpp::Node("ibsen_driver")
    {
        // ---- Parameters (ROS2: declare then get) ----
        this->declare_parameter<float>("integration_time", 10.0f);
        this->declare_parameter<std::string>("wavelength_range", "");
        this->declare_parameter<float>("min_wavelength", 0.0f);
        this->declare_parameter<float>("max_wavelength", 0.0f);

        integrationTime  = this->get_parameter("integration_time").as_double();
        wavelength_range = this->get_parameter("wavelength_range").as_string();
        minWave          = this->get_parameter("min_wavelength").as_double();
        maxWave          = this->get_parameter("max_wavelength").as_double();

        // ---- Publisher + service ----
        pub = this->create_publisher<spectrometer_interfaces::msg::Spectra>("spectral_data", 10);
        service = this->create_service<spectrometer_interfaces::srv::Integration>(
            "set_integration_time",
            std::bind(&IbsenDriver::UpdateIntegrationCallService, this,
                      std::placeholders::_1, std::placeholders::_2));
    }

    // ---- Hardware setup, split out of the constructor so the node exists
    //      as a shared_ptr before we open devices and start spinning. ----
    bool setup()
    {
        // Find device handles to use here
        std::vector<int> possibleHandles = ListFtUSBDevices();
        int startIndex = this->TestDevices(possibleHandles, wavelength_range);
        if (startIndex < 0) {
            RCLCPP_ERROR(this->get_logger(), "No matching spectrometer found; aborting setup.");
            return false;
        }
        this->ftHandleCS0 = (FT_HANDLE)NULL;
        this->ftHandleCS1 = (FT_HANDLE)NULL;
        // Open a connection to the spectrometer based on description for CS1.
        ftStatus = FT_OpenEx((PVOID)(uintptr_t)possibleHandles[startIndex], FT_OPEN_BY_LOCATION, &this->ftHandleCS1);
        if (ftStatus != FT_OK)
        {
            RCLCPP_ERROR(this->get_logger(), "FT_OpenEx failed (error %d)", (int)ftStatus);
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "Device successfully opened with code: %d", (int)ftStatus);
        }
        // Open a connection to the spectrometer based on description for CS0.
        ftStatus = FT_OpenEx((PVOID)(uintptr_t)possibleHandles[startIndex+1], FT_OPEN_BY_LOCATION, &this->ftHandleCS0);
        if (ftStatus != FT_OK)
        {
            RCLCPP_ERROR(this->get_logger(), "FT_OpenEx failed (error %d)", (int)ftStatus);
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "Device successfully opened with code: %d", (int)ftStatus);
        }
        //-------------------------------------------------------------------------------------------------------------
        // SET LATENCY TIMER
        //-------------------------------------------------------------------------------------------------------------
        UCHAR UcLatency = 20;
        ftStatus = FT_SetLatencyTimer(this->ftHandleCS1, UcLatency);
        //-------------------------------------------------------------------------------------------------------------
        // SET USB PARAMETERS
        //-------------------------------------------------------------------------------------------------------------
        ULONG OutTransferSize = 4096;
        ULONG InTransferSize = 65536;
        ftStatus = FT_SetUSBParameters(this->ftHandleCS1, InTransferSize, OutTransferSize);
        //-------------------------------------------------------------------------------------------------------------
        // SET CLOCK  - 60 MHz
        //-------------------------------------------------------------------------------------------------------------
        FT4222_SetClock(this->ftHandleCS1, SYS_CLK_60);
        //-------------------------------------------------------------------------------------------------------------
        // MASTER SPI INITIALIZE
        FT4222_STATUS ft4222Status;
        // Initialize the FT4222H as a master for USB to SPI bridge functionality.
        ft4222Status =
            FT4222_SPIMaster_Init(this->ftHandleCS0, SPI_IO_SINGLE, CLK_DIV_4, CLK_IDLE_LOW, CLK_TRAILING, 2);
        if (FT4222_OK != ft4222Status)
        {
            RCLCPP_ERROR(this->get_logger(), "Init FT4222 as SPI master device failed!");
        }
        else
        {
            RCLCPP_DEBUG(this->get_logger(), "Pointer of Register parameter handle, CS0 in DISB HW manual : %p", this->ftHandleCS0);
        }
        // Initialize the FT4222H as a master for USB to SPI bridge functionality.
        ft4222Status =
            FT4222_SPIMaster_Init(this->ftHandleCS1, SPI_IO_SINGLE, CLK_DIV_4, CLK_IDLE_LOW, CLK_TRAILING, 3);
        if (FT4222_OK != ft4222Status)
        {
            RCLCPP_ERROR(this->get_logger(), "Init FT4222 as SPI master device failed!");
        }
        else
        {
            RCLCPP_DEBUG(this->get_logger(), "Pointer of Bulk data handle, CS1 in DISB HW manual: %p", this->ftHandleCS1);
        }
        // Perform a single read of register 1
        uint16_t PCB_SN = this->ReadRegister(this->ftHandleCS0, 1);
        RCLCPP_INFO(this->get_logger(), "PCB SERIAL NUMBER %d", PCB_SN);
        // Read the HW version of the DISB board, located in register 2.
        uint16_t HardwareVersion = this->ReadRegister(this->ftHandleCS0, 2);
        RCLCPP_INFO(this->get_logger(), "HW_TYPE %d", HardwareVersion);
        // Read the FW version of the DISB board, located in register 3.
        uint16_t FirmwareVersion = this->ReadRegister(this->ftHandleCS0, 3);
        RCLCPP_INFO(this->get_logger(), "FIRMWARE %d", FirmwareVersion);
        // Read the Detector type, located in register 4.
        uint16_t DetectorType = this->ReadRegister(this->ftHandleCS0, 4);
        RCLCPP_INFO(this->get_logger(), "Detector type: %d", DetectorType);
        // Read the number of pixels per image, located in register 5.
        uint16_t PixelPerImage = this->ReadRegister(this->ftHandleCS0, 5);
        RCLCPP_INFO(this->get_logger(), "Pixel per Image: %d", PixelPerImage);
        // Read the number of characters used for calibration coefficients, located in register 6.
        uint16_t NumberofCaliChars = this->ReadRegister(this->ftHandleCS0, 6);
        RCLCPP_INFO(this->get_logger(), "Number of characters of calibration coefficients: %d ", NumberofCaliChars);
        // Read the wavelength calibration coefficients, via register 7.
        RCLCPP_INFO(this->get_logger(), "Calibration coefficients: ");
        double ConvertedCalibrationCoefficients[NumberofCaliChars / 14];
        for (int i = 0; i < NumberofCaliChars / 14; i++)
        {
            char CombinedCalibrationChars[14];
            for (int j = 0; j < 14; j++)
            {
                CombinedCalibrationChars[j] = this->ReadRegister(this->ftHandleCS0, 7);
            }
            RCLCPP_INFO(this->get_logger(), "%s", CombinedCalibrationChars);
            ConvertedCalibrationCoefficients[i] = std::strtod(CombinedCalibrationChars, NULL);
        }
        // Read the temperature register number 11.
        uint16_t Temperature = this->ReadRegister(this->ftHandleCS0, 11);
        RCLCPP_INFO(this->get_logger(), "Starting temperature measured: %d", Temperature);
        // Read the Trigger delay, located in register 13 and 14.
        uint16_t TriggerDelayLSB = this->ReadRegister(this->ftHandleCS0, 13);
        uint16_t TriggerDelayMSB = this->ReadRegister(this->ftHandleCS0, 14);
        uint32_t TriggerDelay = (TriggerDelayLSB << 16) | (TriggerDelayMSB & 0xffff);
        RCLCPP_INFO(this->get_logger(), "Trigger delay: %d", TriggerDelay);
        // Read the Amplifier Gain, located in register 15.
        uint16_t ADCGain = this->ReadRegister(this->ftHandleCS0, 15);
        RCLCPP_INFO(this->get_logger(), "ADC Programmable Gain Amplification (PGA) number: %d", ADCGain);
        // Read the Amplifier offset, located in register 16.
        uint16_t ADCOffset = this->ReadRegister(this->ftHandleCS0, 16);
        RCLCPP_INFO(this->get_logger(), "ADC offset number: %d", ADCOffset);
        // Read the Spectrometer serial number, located in register 22.
        uint16_t SpectrometerSN = this->ReadRegister(this->ftHandleCS0, 22);
        RCLCPP_INFO(this->get_logger(), "Spectrometer serial number: %d", SpectrometerSN);

        this->FirstPixel = this->ReadRegister(this->ftHandleCS0, 23);
        RCLCPP_INFO(this->get_logger(), "First pixel read: %d", this->FirstPixel);
        // Read the last pixel to be read, located in register 24.
        this->LastPixel = this->ReadRegister(this->ftHandleCS0, 24);
        RCLCPP_INFO(this->get_logger(), "Last pixel read: %d", this->LastPixel);
        // Set starting integration time
        this->SetIntegrationTime(integrationTime);
        // Create the wavelengths
        this->wavelengths = this->GenerateWavelengths(ConvertedCalibrationCoefficients, PixelPerImage);
        return true;
    }

    // ---- Main acquisition loop (was inside the ROS1 constructor's while) ----
    void run()
    {
        while (rclcpp::ok())
        {
            // Writing to the Sensor Control register 8.
            // Perform soft reset of the image transfer buffer
            this->SetRegister(this->ftHandleCS0, 8, 0x10);
            // Trigger an exposure
            this->SetRegister(this->ftHandleCS0, 8, 0x01);

            // The two bytes are then stitched together to form the correct pixel values.
            uint16 datalength = (this->LastPixel - this->FirstPixel + 1.0) * 2.0;
            std::vector<uint8> NewReadFrambuffer{ 0 };
            NewReadFrambuffer.resize(datalength);
            std::vector<uint8> NewWriteFrambuffer{ 0 };
            NewWriteFrambuffer.resize(datalength);

            // Wait for the framebuffer to be full (NumberOfPixReady = LastPixel) or for timeout after 1 second.
            clock_t StartTime = clock();
            clock_t CurrentTime = clock();
            int TimeElapsedinSec;
            uint16_t NumberOfPixReady;
            do {
                NumberOfPixReady = ReadRegister(ftHandleCS0, 12);
                CurrentTime = clock();
                TimeElapsedinSec = (CurrentTime - StartTime) / CLOCKS_PER_SEC;
            } while (NumberOfPixReady - this->LastPixel < 0 && TimeElapsedinSec < 1);
            if (TimeElapsedinSec >= 1)
            {
                RCLCPP_WARN(this->get_logger(), "Timeout error when capturing spectrum");
            }

            uint16_t SizeTransferred;
            // Read the pixel values
            FT4222_SPIMaster_SingleReadWrite(ftHandleCS1, NewReadFrambuffer.data(), NewWriteFrambuffer.data(), datalength, &SizeTransferred, 1);

            // Stitch the pixel values
            std::vector<float> pixels;
            spectrometer_interfaces::msg::Spectra msg;
            uint16_t SpectrometerPix;
            for (int i = 0; i < datalength; i += 2) {
                SpectrometerPix = (NewReadFrambuffer[i] << 8) | (NewReadFrambuffer[i + 1] & 0xff);
                float SpectrometerPixCov = SpectrometerPix;
                pixels.push_back(SpectrometerPixCov);
            }
            // Extract only valid pixels from the range
            if (strcmp(this->wavelength_range.c_str(),"nir") == 0) {
                std::vector<float> finalData = {pixels.begin() + 62, pixels.begin() + 190};
                std::reverse(finalData.begin(), finalData.end());
                msg.data = finalData;
            } else {
                std::reverse(pixels.begin(), pixels.end());
                msg.data = pixels;
            }
            // Grab current temperature
            msg.temp = (int)this->ReadRegister(this->ftHandleCS0, 11);
            // Set integration time here (snake_case field in ROS2 interface)
            msg.integration_time = this->integrationTime;
            // Set linear space wavelengths for reference/plots
            msg.wavelengths = this->wavelengths;
            pub->publish(msg);
            // Sleep for the integration time
            rclcpp::sleep_for(std::chrono::milliseconds((int)this->integrationTime));
            rclcpp::spin_some(this->get_node_base_interface());
        }
    }

    void shutdown()
    {
        FT4222_UnInitialize(this->ftHandleCS0);
        FT4222_UnInitialize(this->ftHandleCS1);
        FT_Close(this->ftHandleCS0);
        FT_Close(this->ftHandleCS1);
    }

    ~IbsenDriver()
    {
        // Uninitialize Device Master SPI and close the FT4222 handles.
        FT4222_UnInitialize(this->ftHandleCS0);
        FT4222_UnInitialize(this->ftHandleCS1);
        FT_Close(this->ftHandleCS0);
        FT_Close(this->ftHandleCS1);
    }

private:
    rclcpp::Publisher<spectrometer_interfaces::msg::Spectra>::SharedPtr pub;
    rclcpp::Service<spectrometer_interfaces::srv::Integration>::SharedPtr service;

    float integrationTime;
    std::string wavelength_range;
    float minWave;
    float maxWave;
    std::vector<float> wavelengths;
    FT_STATUS ftStatus;
    FT_HANDLE ftHandleCS0;
    FT_HANDLE ftHandleCS1;
    uint16_t FirstPixel;
    uint16_t LastPixel;

    std::vector<int> ListFtUSBDevices()
    {
        DWORD numDevs = 0;
        int i;
        FT_STATUS ftStatus;
        std::vector<int> devices;
        // Check number of devices
        ftStatus = FT_CreateDeviceInfoList(&numDevs);
        printf("%d", ftStatus);
        // Cycle through the different available device, there should be 4, (A-D)
        if (ftStatus == 0)
        {
            for (i = 0; i < (int)numDevs; i++)
            {
                FT_DEVICE_LIST_INFO_NODE devInfo;
                memset(&devInfo, 0, sizeof(devInfo));
                ftStatus =
                    FT_GetDeviceInfoDetail(i, &devInfo.Flags, &devInfo.Type, &devInfo.ID, &devInfo.LocId,
                                           devInfo.SerialNumber, devInfo.Description, &devInfo.ftHandle);
                printf("\nDevice %d: Description '%s': LocationID '%d':", i, devInfo.Description,
                       devInfo.LocId);
                if (strcmp(devInfo.Description,"FT4222 A") == 0) {
                    devices.push_back(devInfo.LocId);
                }
                if (strcmp(devInfo.Description,"FT4222 B") == 0) {
                    devices.push_back(devInfo.LocId);
                }
            }
        }
        else
        {
            printf("Error, No FT4222H detected.\n");
        }
        return devices;
    }

    // Use the device's wavelength calibration coefficients to generate the central wavelength associated with each pixel
    std::vector<float> GenerateWavelengths(double Calibration[], std::size_t N)
    {
        std::vector<float> xs(N);
        std::vector<float> xs_extract(128);
        for (double i = 0; i < N; i++) {
            xs[i] = Calibration[0] + Calibration[1]*i + Calibration[2]*pow(i,2) + Calibration[3]*pow(i,3) + Calibration[4]*pow(i,4);
        }
        for (float i: xs)
            printf("%f",i);
        if (strcmp(this->wavelength_range.c_str(),"nir") == 0) {
            std::copy_if (xs.begin(), xs.end(), std::back_inserter(xs_extract), [](float i){return i>=900 && i<=1702;} );
            xs_extract.erase(
                std::remove(xs_extract.begin(), xs_extract.end(), 0),
                xs_extract.end());
            xs_extract.shrink_to_fit();
            xs = xs_extract;
        }
        std::reverse(xs.begin(), xs.end());
        return xs;
    }

    void UpdateIntegrationCallService(
        const std::shared_ptr<spectrometer_interfaces::srv::Integration::Request> req,
        std::shared_ptr<spectrometer_interfaces::srv::Integration::Response> res)
    {
        // Receive a request and update the integration time in milliseconds
        res->response = true;
        try
        {
            this->SetIntegrationTime(req->data);
        }
        catch(const std::exception& e)
        {
            res->response = false;
        }
    }

    // Calculate the integration time
    // @param newTime is the integration time in milliseconds
    void SetIntegrationTime(float newTime) {
        this->integrationTime = newTime;
        int useTime = this->CalculateIntegrationTime(newTime);
        unsigned int lsb = useTime & 0xFFFF;
        unsigned int msb = (useTime >> 16) & 0xFFFF;
        this->SetRegister(this->ftHandleCS0, 10, msb);
        this->SetRegister(this->ftHandleCS0, 9, lsb);
    }

    int CalculateIntegrationTime(float requestTime) {
        double increments = (double)requestTime * 1000000 / 200;
        return (int)increments;
    }

    // Read value of DISB register
    uint16_t ReadRegister(PVOID FThandle, int RegisterAddress)
    {
        uint8_t AppendedRegAddress = RegisterAddress * 4 + 2;
        uint8 ReadBuffer[3], WriteBuffer[3];
        uint16 SizeTransferred;
        WriteBuffer[0] = AppendedRegAddress;
        WriteBuffer[1] = 0x0;
        WriteBuffer[2] = 0x0;
        FT4222_SPIMaster_SingleReadWrite(FThandle, ReadBuffer, WriteBuffer, 3, &SizeTransferred, 1);
        uint16_t RegisterValue = (ReadBuffer[1] << 8) | (ReadBuffer[2] & 0xff);
        return RegisterValue;
    }

    // Set a new DISB Register value
    int SetRegister(PVOID FThandle, int RegisterAddress, int NewRegisterValue)
    {
        uint8_t AppendedRegAddress = RegisterAddress * 4;
        uint8 ReadBuffer[3], WriteBuffer[3], MSB, LSB;
        uint16 SizeTransferred;
        WriteBuffer[0] = AppendedRegAddress;
        MSB = NewRegisterValue >> 8;
        LSB = NewRegisterValue;
        WriteBuffer[1] = MSB;
        WriteBuffer[2] = LSB;
        FT4222_SPIMaster_SingleReadWrite(FThandle, ReadBuffer, WriteBuffer, 3, &SizeTransferred, 1);
        return 0;
    }

    // Given a series of device handles, connect to the device and extract important metadata
    std::vector<float> GetDevice(int device_loc_1, int device_loc_2) {
        FT_STATUS   ftStatus;
        FT_HANDLE	ftHandleCS0 = (FT_HANDLE)NULL;
        FT_HANDLE	ftHandleCS1 = (FT_HANDLE)NULL;

        ftStatus = FT_OpenEx((PVOID)(uintptr_t)device_loc_1, FT_OPEN_BY_LOCATION, &ftHandleCS1);
        if (ftStatus != FT_OK) {
            printf("\nFT_OpenEx failed (error %d)\n", (int)ftStatus);
        }
        ftStatus = FT_OpenEx((PVOID)(uintptr_t)device_loc_2, FT_OPEN_BY_LOCATION, &ftHandleCS0);
        if (ftStatus != FT_OK){
            printf("\nFT_OpenEx failed (error %d)\n", (int)ftStatus);
        }
        UCHAR UcLatency = 20;
        ftStatus = FT_SetLatencyTimer(ftHandleCS1, UcLatency);
        ULONG OutTransferSize = 4096;
        ULONG InTransferSize = 65536;
        ftStatus = FT_SetUSBParameters(ftHandleCS1, InTransferSize, OutTransferSize);
        FT4222_SetClock(ftHandleCS1, SYS_CLK_60);
        FT4222_STATUS ft4222Status;
        ft4222Status = FT4222_SPIMaster_Init(ftHandleCS0, SPI_IO_SINGLE,
                        CLK_DIV_4, CLK_IDLE_LOW,
                        CLK_TRAILING, 2);
        if (FT4222_OK != ft4222Status) {
            printf("Init FT4222 as SPI master device failed!\n");
        }
        ft4222Status = FT4222_SPIMaster_Init(ftHandleCS1, SPI_IO_SINGLE,
                        CLK_DIV_4, CLK_IDLE_LOW,
                        CLK_TRAILING, 3);
        if (FT4222_OK != ft4222Status) {
            printf("Init FT4222 as SPI master device failed!\n");
        }
        uint16_t NumberofCaliChars = ReadRegister(ftHandleCS0, 6);
        double ConvertedCalibrationCoefficients[NumberofCaliChars / 14];
        for (int i = 0; i < NumberofCaliChars / 14; i++)
            {
            char CombinedCalibrationChars[14];
            for (int j = 0; j < 14; j++)
            {
            CombinedCalibrationChars[j] = ReadRegister(ftHandleCS0, 7);
            }
            ConvertedCalibrationCoefficients[i] = std::strtod(CombinedCalibrationChars, NULL);
        }
        uint16_t PixelPerImage = ReadRegister(ftHandleCS0, 5);
        std::vector<float> wavelengths = GenerateWavelengths(ConvertedCalibrationCoefficients, PixelPerImage);
        FT4222_UnInitialize(ftHandleCS0);
        FT4222_UnInitialize(ftHandleCS1);
        FT_Close(ftHandleCS0);
        FT_Close(ftHandleCS1);
        return wavelengths;
    }

    int TestDevices(std::vector<int> devices, std::string target) {
        printf("NUMBER OF DEVICES: %d",(int)devices.size());
        if (devices.size() % 2 != 0) {
            printf("INVALID NUMBER OF DETECTED DEVICES");
            return -1;
        }
        for (int i=0; i<(int)devices.size()-1; i=i+2) {
            std::vector<float> waves = this->GetDevice(devices[i],devices[i+1]);
            int minVal = *std::min_element(waves.begin(), waves.end());
            int maxVal = *std::max_element(waves.begin(), waves.end());
            printf("%d\n", minVal);
            printf("%d\n", maxVal);
            std::string currentDevice;
            if (minVal < 460 && maxVal < 1105) {
                currentDevice = "vnir";
            } else {
                currentDevice = "nir";
            }
            if (strcmp(target.c_str(),currentDevice.c_str()) == 0) {
                return i;
            }
        }
        RCLCPP_ERROR(this->get_logger(), "NO MATCHES FOR DESIRED SPECTROMETER TYPE!");
        return -1;
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<IbsenDriver>();
    if (node->setup()) {
        node->run();
    }
    node->shutdown();
    rclcpp::shutdown();
    return 0;
}
