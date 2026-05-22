SAMPLES=" \
  GenICamParameters \
  MulticastMaster \
  MulticastSlave \
  MultiSource \
  PvPipelineSample \
  PvStreamSample \
  DeviceSerialPort \
  TransmitTestPattern \
  ConnectionRecovery \
  TapReconstruction \
  TransmitChunkData \
  ConfigurationReader \
  CameraBridge \
  SoftDeviceGEV \
  SoftDeviceGEVChunkData \
  SoftDeviceGEVSimple \
  SoftDeviceGEVTrigger \
  SoftDeviceGEVMultiPart \
  DeviceFinder \
  EventSample \
  eBUSPlayer \
  eBUSPlayer \
  DualSource \
  TransmitProcessedImage \
  ImageProcessing \
"

for SAMPLE in $SAMPLES; do
  make -C $SAMPLE
done

