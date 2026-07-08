from ximea import xiapi
import cv2     

#create instance for first connected camera 
cam = xiapi.Camera()

#start communication
print('Opening first camera...')
cam.open_device()

#settings
cam.set_exposure(20000)

cam.enable_auto_wb()

#create instance of Image to store image data and metadata
img = xiapi.Image()

#start data acquisition
print('Starting data acquisition...')
cam.start_acquisition()

#get data and pass them from camera to img
cam.get_image(img)

#create numpy array with data from camera. Dimensions of array are determined
#by imgdataformat
data = img.get_image_data_numpy()

#stop data acquisition
print('Stopping acquisition...')
cam.stop_acquisition()

#stop communication
cam.close_device()

#show acquired image
print('Drawing image...')
h, w = data.shape[:2] # Get image dimensions
max_w, max_h = 1920, 1280 # Maximum window size
scale = min(max_w / w, max_h / h) # Calculate scale factor
win_w, win_h = int(w * scale), int(h * scale) # Resized dimensions

win = 'OpenCV B&W example'
cv2.namedWindow(win, cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)  # Resizable
cv2.resizeWindow(win, win_w, win_h)  # Risezing window and image
cv2.imshow(win, data)
cv2.waitKey(0)
cv2.destroyAllWindows()

print('Done.')
