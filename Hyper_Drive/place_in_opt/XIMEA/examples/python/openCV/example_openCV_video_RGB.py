from ximea import xiapi
import cv2
import time


def get_camera_basic_info(cam: xiapi.Camera) -> str:
    """
    The function returns basic information about the camera and API version.
    
    :return device name, serial number, sensor serial number, api version:
    """
    if cam.is_isexist:
        name = cam.get_device_name().decode("UTF-8")
        sn = cam.get_device_sn().decode("UTF-8")
        try:
            sensor_sn = cam.get_device_sens_sn().decode("UTF-8")
        except Exception as e:
            sensor_sn = e
        api_version = cam.get_api_version().decode("UTF-8")

        return f"""
        Device Name:\t\t\t{name}
        Serial Number:\t\t\t{sn}
        Sensor Serial Number:\t\t{sensor_sn}
        Api Version:\t\t\t{api_version}
        """

#create instance for first connected camera 
cam = xiapi.Camera()

#start communication
print('Opening first camera...')
cam.open_device()
info = get_camera_basic_info(cam)
print(info)

#settings
cam.set_imgdataformat('XI_RGB24')
cam.set_exposure(20000)

cam.enable_auto_wb()

#create instance of Image to store image data and metadata
img = xiapi.Image()

#start data acquisition
print('Starting data acquisition...')
cam.start_acquisition()

try:
    print('Starting video. Press ESC to exit.')
    t0 = time.time()
    while True:
        #get data and pass them from camera to img
        cam.get_image(img)

        #create numpy array with data from camera. Dimensions of the array are 
        #determined by imgdataformat
        data = img.get_image_data_numpy()

        #show acquired image with time since the beginning of acquisition
        font = cv2.FONT_HERSHEY_SIMPLEX
        text = '{:5.2f}'.format(time.time()-t0)
        cv2.putText(
            data, text, (900,150), font, 4, (255, 255, 255), 2
            )
        
        h, w = data.shape[:2] # Get image dimensions
        max_w, max_h = 1920, 1280 # Maximum window size
        scale = min(max_w / w, max_h / h) # Calculate scale factor
        win_w, win_h = int(w * scale), int(h * scale) # Resized dimensions

        win = 'OpenCV RGB video example'
        cv2.namedWindow(win, cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)  # Resizable
        cv2.resizeWindow(win, win_w, win_h)  # Risezing window and image
        cv2.imshow(win, data)

        key = cv2.waitKey(1)
        if key == 27:
            break
        
except KeyboardInterrupt:
    cv2.destroyAllWindows()

#stop data acquisition
print('Stopping acquisition...')
cam.stop_acquisition()

#stop communication
cam.close_device()

print('Done.')
