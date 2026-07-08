Module VB_example

    ' save image function
    Sub saveImage(ByRef image As System.Windows.Media.Imaging.BitmapSource, ByVal index As Integer)

        Dim width As Integer = image.Width
        Dim height As Integer = image.Height
        Dim stride As Integer = CType(width / 8, Integer)
        Dim pixels(height * stride) As Byte

        ' Define the image palette
        Dim myPalette As System.Windows.Media.Imaging.BitmapPalette = System.Windows.Media.Imaging.BitmapPalettes.Halftone256

        ' Creates a new empty image with the pre-defined palette
        Dim name As String = String.Format("images\image_{0}.jpg", index)
        Dim stream As New System.IO.FileStream(name, System.IO.FileMode.Create)
        Dim encoder As New System.Windows.Media.Imaging.JpegBitmapEncoder()
        encoder.FlipHorizontal = False
        encoder.FlipVertical = False
        encoder.QualityLevel = 100
        encoder.Frames.Add(System.Windows.Media.Imaging.BitmapFrame.Create(image))
        encoder.Save(stream)

    End Sub

    ' main program loop
    Sub Main()

        ' create folder to store images
        If (Not System.IO.Directory.Exists("images")) Then
            System.IO.Directory.CreateDirectory("images")
        End If

        Console.WriteLine("xiAPI.NET VB example")
        Dim camera As New xiCam

        Try
            ' initialize first camera
            Dim numCams As Integer = camera.GetNumberDevices()
            Console.WriteLine("Found {0} XIMEA cameras", numCams)

            If numCams = 0 Then
                Console.WriteLine("No cameras found...")
                Console.ReadLine()
                Return
            End If

            ' open first device
            camera.OpenDevice(0)

            ' identify device
            Console.WriteLine("Got camera {0} SN {1}", camera.GetParamString(PRM.DEVICE_NAME), camera.GetParamString(PRM.DEVICE_SN))

            ' set exposure to 2ms
            Dim exposure As Integer = 2000
            camera.SetParam(PRM.EXPOSURE, exposure)

            ' set gain to 5 dB
            Dim gain As Single = 5.0
            camera.SetParam(PRM.GAIN, gain)

            ' start acquisition
            camera.StartAcquisition()
            Dim timeout As Integer = 1000

            ' acquire images
            For index As Integer = 0 To 100 Step 1
                Dim image As System.Windows.Media.Imaging.BitmapSource
                camera.GetBitmapSourceByRef(image, timeout)
                Console.WriteLine("Got image {0}. {1}x{2} ", index, image.Width, image.Height)
                saveImage(image, index)
            Next
        Catch ex As xiExc
            Console.WriteLine(ex.Message)
        Finally
            ' close camera
            camera.StopAcquisition()
            camera.CloseDevice()
        End Try

        Console.WriteLine("Press any key to terminate application...")
        Console.ReadLine()

    End Sub

End Module