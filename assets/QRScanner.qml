import bb.cascades 1.0
import bb.cascades.multimedia 1.0
import bb.multimedia 1.0

Page {
    signal canceled()
    titleBar: TitleBar {
        dismissAction: ActionItem {
            title: qsTr("Cancel")
            onTriggered: {
                canceled()
            }
        }
        title: qsTr("Scan QR Code")
    }
    Container {
        layout: DockLayout {
        }
        Camera {
            id: camera

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            onCameraOpened: {
                // Apply some settings after the camera was opened successfully
                getSettings(cameraSettings)
                cameraSettings.focusMode = CameraFocusMode.ContinuousAuto
                cameraSettings.shootingMode = CameraShootingMode.Stabilization
                applySettings(cameraSettings)

                // Start the view finder as it is needed by the barcode detector
                camera.startViewfinder()
            }

            onViewfinderStarted: {
                // Setup the barcode detector with the camera object now
                barcodeDetector.camera = camera
            }

            attachedObjects: [
                CameraSettings {
                    id: cameraSettings
                }
            ]
        }

        // The overlay image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/overlay.png"
        }
        attachedObjects: [
            BarcodeDetector {
                id: barcodeDetector
                formats: BarcodeFormat.QrCode
                onDetected: {
                    console.log(data)
                }
            },
            SystemSound {
                id: scannedSound

                sound: SystemSound.GeneralNotification
            }
        ]
    }
    function start(){
        camera.open()
    }
    function stop(){
        camera.close()
    }
}
