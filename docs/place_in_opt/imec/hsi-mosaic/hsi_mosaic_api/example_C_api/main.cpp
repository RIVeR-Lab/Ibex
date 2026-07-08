#include <hsi_mosaic_api.h>
#include <wchar.h>
#include <deque>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
// forward declarations
void DisplayResult (std::string const& i_message, HSI_RETURN i_result);

/*!
\brief Structure gathering all inputs required to run the example.
*/
struct TestConfiguration
{
    std::string context_path;
    std::string calibration_file;
    std::string non_uniformity_bright;
    std::string non_uniformity_dark;
    std::deque <std::string> dark_references;
    OpticalSetup optical_setup;
    std::string data_frame;
    RegionOfInterest output_size;
    RegionOfInterest reference_roi;
    std::string output_dir;
    std::string log_dir;
};

void Example (TestConfiguration);

int main (int i_nr_args, char* i_args [])
{
    // check if the example resource is available
    FILE* file_pointer = fopen ("../../example_resources/ApplesBelgica/context/calibration_file/CMV2K-SSM4x4-600_850-13.13.3.8_refl_radiom.xml", "r");
    if (nullptr == file_pointer) 
    {
        std::cout << "The resources for this example code cannot be found. Please download it from the customer site." << std::endl;
        int i = system ("pause");
        exit (-1);
    }
    else 
    {
        // resource exist, close and continue
        fclose (file_pointer);
    }

    // fill in the parameters to run the example code
    TestConfiguration configuration = {};

    configuration.context_path          = "../../example_resources/ApplesBelgica/context";
    configuration.calibration_file      = configuration.context_path + "/calibration_file/CMV2K-SSM4x4-600_850-13.13.3.8_refl_radiom.xml";
    configuration.non_uniformity_bright = configuration.context_path + "/non_uniformity/white_reference.raw.xml";
    configuration.non_uniformity_dark   = configuration.context_path + "/non_uniformity/dark_reference.raw.xml";
    configuration.dark_references = 
    {
        configuration.context_path + "/dark_references/dark_reference_4.000000.raw.xml",
    };
    configuration.data_frame            = configuration.context_path + "/../Acquisition0/image_0.raw.xml";
    configuration.optical_setup = {
        4.0,
        16,
        21,
        const_cast<char*>("Edmund Optics"),
        OR_VISNIR
    };
    configuration.output_size = { 0, 0, 1020, 540 };
    configuration.reference_roi = configuration.output_size;
    configuration.output_dir = "./output/";
    configuration.log_dir = "./logs/";

    // run the example
    Example (configuration);
}

void Example (TestConfiguration i_configuration)
{
    // setup logger
    commonInitializeLogger (i_configuration.log_dir.c_str (), LoggerVerbosity::LV_WARNING);

    // prepare for building the context
    HANDLE context = nullptr;
    FrameFloat non_uniformity_bright = {};
    FrameFloat non_uniformity_dark = {};
    std::vector <FrameFloat> dark_field_references (i_configuration.dark_references.size ());

    //-- pre-loading the data frame : the frame that needs to be converted into a cube.
    FrameFloat data_frame = {};
    DisplayResult("LoadFrame()", commonLoadFrame (&data_frame, i_configuration.data_frame.c_str ()));

    if (true)
    {
        //-- here the entire context is loaded with a single call, including the calibration file
        //-- the dark references, the non-uniformity data and the optical setup configuration.
        DisplayResult ("LoadContext()", mosaicLoadContext(&context, i_configuration.context_path.c_str()));
    }
    else
    {
        //-- here the context is build up by context-specific API calls. With these calls an entire
        //-- and valid context can be created and used.
        //
        // . allocate a new context
        DisplayResult("AllocateContext()", mosaicAllocateContext (&context, data_frame));
        // . add the calibration file
        DisplayResult("ContextSetCalibrationFile()", mosaicContextSetCalibrationFile (context, i_configuration.calibration_file.c_str ()));
        // . add the dark references
        for (int i=0; i<dark_field_references.size (); ++i)
        {
            DisplayResult("LoadFrame()", commonLoadFrame (&dark_field_references[i], i_configuration.dark_references[i].c_str ()));
        }
        DisplayResult("ContextSetDarkFieldReferences()", mosaicContextSetDarkFieldReferences (context, dark_field_references.data (), dark_field_references.size ()));
        // . add the non uniformity data
        DisplayResult("LoadFrame()", commonLoadFrame (&non_uniformity_bright, i_configuration.non_uniformity_bright.c_str ()));
        DisplayResult("LoadFrame()", commonLoadFrame (&non_uniformity_dark, i_configuration.non_uniformity_dark.c_str ()));
        DisplayResult("ContextSetNonUniformity()", mosaicContextSetNonUniformity (context, &non_uniformity_bright, &non_uniformity_dark));
        // . add the optical setup configuration
        DisplayResult("ContextSetOpticalSetup()", mosaicContextSetOpticalSetup (context, i_configuration.optical_setup));
    }

    // set some pipeline configuration parameters
    mosaicConfigurationParameters parameters = {};
    parameters.spatial_median_filter_enable = true;
    parameters.spatial_median_filter_kernel_size = KS_3X3;
    parameters.spatial_resampling_width = i_configuration.output_size.width;
    parameters.spatial_resampling_height = i_configuration.output_size.height;
    parameters.white_balance_enable = false;

    // we need some variables along the way
    HANDLE pipeline = 0x0;
    CubeDataFormat output_data_format {};
    CubeFloat data_cube = {};
    SpectrumFloat reference_spectrum = {};

    // build and configure pipeline
    DisplayResult("Create()", mosaicCreate (&pipeline, context));
    DisplayResult("SetConfigurationParameters()", mosaicSetConfigurationParameters (pipeline, parameters));

    DisplayResult("Initialize()", mosaicInitialize (pipeline));
    
    // get the default reference spectrum and apply it
    DisplayResult("GetOutputDataFormat()", mosaicGetOutputDataFormat (pipeline, &output_data_format));
    DisplayResult("AllocateSpectrum()", commonAllocateSpectrum (&reference_spectrum, output_data_format));
    DisplayResult("DeallocateCubeDataFormat()", commonDeallocateCubeDataFormat (&output_data_format));
    DisplayResult("Stop()", mosaicStop (pipeline));

    // re-initialize and re-start the pipeline (with reference included now)
    DisplayResult("SetConfigurationParameters()", mosaicSetConfigurationParameters (pipeline, parameters));
    DisplayResult("Initialize()", mosaicInitialize (pipeline));

    DisplayResult("GetOutputDataFormat()", mosaicGetOutputDataFormat (pipeline, &output_data_format));
    DisplayResult("Start()", mosaicStart (pipeline));

    // allocate data
    DisplayResult("AllocateCube()", commonAllocateCube (&data_cube, output_data_format));
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    

    for(int i = 0; i < 1000; i++) {
        auto t1 = high_resolution_clock::now();
        DisplayResult("PushFrame()", mosaicPushFrame (pipeline, data_frame));
        DisplayResult("GetCube()", mosaicGetCube (pipeline, &data_cube, 1000000));
        auto t2 = high_resolution_clock::now();
        auto ms_int = duration_cast<milliseconds>(t2 - t1);
        std::cout << ms_int.count() << "ms\n";


    }
    // shutdown
    DisplayResult("Pause()", mosaicPause (pipeline));
    DisplayResult("Stop()", mosaicStop (pipeline));

    // get reference spectrum
    RegionOfInterest roi = {10,10,10,10};
    DisplayResult("ExtractSpectrumFromCube", mosaicExtractSpectrumFromCube(&reference_spectrum, data_cube, roi));
    DisplayResult("SetReferenceSpectrum()", mosaicSetReferenceSpectrum(pipeline, reference_spectrum, 0.50));

    // re-initialize
    DisplayResult("Initialize()", mosaicInitialize (pipeline));

    DisplayResult("GetOutputDataFormat()", mosaicGetOutputDataFormat (pipeline, &output_data_format));
    DisplayResult("Start()", mosaicStart (pipeline));

    // allocate data
    DisplayResult("DeallocateCube()", commonDeallocateCube (&data_cube));
    DisplayResult("AllocateCube()", commonAllocateCube (&data_cube, output_data_format));
    DisplayResult("PushFrame()", mosaicPushFrame (pipeline, data_frame));
    DisplayResult("GetCube()", mosaicGetCube (pipeline, &data_cube, 1000000));
    
    // save cube
    DisplayResult("SaveCube()", commonSaveCube (data_cube, i_configuration.output_dir.c_str (), "data_cube", FF_ENVI));

    // shutdown
    DisplayResult("Pause()", mosaicPause (pipeline));
    DisplayResult("Stop()", mosaicStop (pipeline));

    // cleanup
    DisplayResult("DeallocateCube()", commonDeallocateCube (&data_cube));
    DisplayResult("DeallocateSpectrum()", commonDeallocateSpectrum (&reference_spectrum));
    DisplayResult("DeallocateCubeDataFormat()", commonDeallocateCubeDataFormat (&output_data_format));
    DisplayResult("DeallocateFrame()", commonDeallocateFrame (&data_frame));

    for (int i = 0; i < dark_field_references.size (); ++i)
    {
        DisplayResult("DeallocateFrame()", commonDeallocateFrame (&dark_field_references [i]));
    }
    DisplayResult("DeallocateFrame()", commonDeallocateFrame (&non_uniformity_dark));
    DisplayResult("DeallocateFrame()", commonDeallocateFrame (&non_uniformity_bright));
    DisplayResult("DeallocateContext()", mosaicDeallocateContext (&context));

    DisplayResult("Destroy()", mosaicDestroy (&pipeline));
}

void DisplayResult (std::string const& i_message, HSI_RETURN i_result)
{
    std::cout << "*** " << std::setw(35) << i_message << ": " << i_result << " => ";
    
    switch (i_result)
    {
    case HSI_OK:                          std::cout << "Function call successful.\n"; break;
    case HSI_HANDLE_INVALID:              std::cout << "Invalid device handle specified.\n"; break;
    case HSI_ARGUMENT_INVALID:            std::cout << "Invalid argument provided in function call.\n"; break;
    case HSI_CALL_ILLEGAL:                std::cout << "Function call illegal given the current internal state.\n"; break;
    case HSI_FILE_NOT_FOUND:              std::cout << "A file could not be found.\n"; break;
    case HSI_CALIBRATION_FILE_NOT_FOUND:  std::cout << "Sensor calibration file could not be found.\n"; break;
    case HSI_CONNECTION_FAILED:           std::cout << "Camera could not be connected to the system.\n"; break;
    case HSI_ALLOCATION_ERROR:            std::cout << "Allocation of resources failed.\n"; break;
    case HSI_ACQUISITION_TIMEOUT:         std::cout << "An Acquisition timeout has occured.\n"; break;
    case HSI_ACQUISITION_FAILED:          std::cout << "Acquisition failed during operation.\n"; break;
    case HSI_DATA_NOT_ALLOCATED:          std::cout << "Provided data structure is not allocated.\n"; break;
    case HSI_DATA_NOT_VALID:              std::cout << "Data with valid flag false provided as input for operation.\n"; break;
    case HSI_DATA_NOT_COMPATIBLE:         std::cout << "Data provided is not compatible.\n"; break;
    case HSI_FILE_SYSTEM_ERROR:           std::cout << "Specified directory doesn't exist and could not be created.\n"; break;
    case HSI_FILE_IO_ERROR:               std::cout << "Could not read or write data from the filesystem.\n"; break;
    case HSI_INTERNAL_ERROR:              std::cout << "An unexpected internal error occurred.\n"; break;
    }
}
