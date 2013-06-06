import bb.cascades 1.0
import bb.cascades.multimedia 1.0
import bb.multimedia 1.0
Sheet {
    id: sheet
    Page {
        titleBar: TitleBar {
            dismissAction: ActionItem {
                title: qsTr("Cancel")
                onTriggered: {
                    _scanner.deleteLater()
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

                onViewfinderStopped: {
                    console.log("Camera Viewfinder Stopped")
                    // camera.close()
                }
                
                onCameraClosed: {
                    console.log("Camera Closed")
                    // _scanner.deleteLater()
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
    }
    onOpened: {
        camera.open()
    }
    onClosed: {
        console.log("Sheet Closed")
        // camera.stopViewfinder()
    }
}