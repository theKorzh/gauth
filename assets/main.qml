// Application with the Sheet project template
import bb.cascades 1.0
import bb.system 1.0

Page {
    id: page
    actions: [
        ActionItem {
            title: qsTr("How it work")
            imageSource: "asset:///icons/ic_help.png"
            ActionBar.placement: ActionBarPlacement.OnBar
        },
        ActionItem {
            id: actionAdd
            title: qsTr("Add account")
            imageSource: "asset:///icons/ic_add.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: [
                SystemShortcut {
                    type: SystemShortcuts.CreateNew
                    onTriggered: {
                        actionAdd.triggered();
                    }
                }
            ]
            onTriggered: {
                menu.show()
            }
            attachedObjects: [
                SystemDialog {
                    id: menu
                    title: "Add Account"
                    body: "How can I help you add new Account"
                    confirmButton {
                        enabled: true
                        label: qsTr("Add Account Manually")
                    }
                    cancelButton {
                        enabled: true
                        label: qsTr("Cancel")
                    }
                    customButton {
                        enabled: true
                        label: qsTr("Scan QR Barcode")
                    }
                    onFinished: {
                        if (value == SystemUiResult.ConfirmButtonSelection) {
                            sheetNew.open()
                        } else if (value == SystemUiResult.CustomButtonSelection) {
                            testDialog.body = "Custom"
                        }
                    }
                },
                SystemDialog {
                    id: confirmDialog
                    title: qsTr("Confirm Deletion")
                    body: qsTr("Confirm Deletion")
                    onFinished: {
                        if (value == SystemUiResult.ConfirmButtonSelection) {
                            if (_app.remove(listView.dataModel.data(listView.activeItem).id)) {
                                listView.dataModel.removeAt(listView.activeItem)
                            }
                        }
                    }
                }
            ]
        }
    ]
    attachedObjects: [
        // sheet is not visible still, prepare it and append to the attachedObjects
        // attachedObjects property is a storage for objects which are not supposed to be visible on scene like data models etc.
        Sheet {
            id: sheetNew
            // sheet GUI appearence component is defined in external CommentSheet.qml file
            content: NewEntry {
                onDone: {
                    sheetNew.close();
                }
            }
            onOpened: {
                content.reset()
            }
        }
    ]
    content: Container {
        layout: DockLayout {

        }
        Container {
            Label {
                text: qsTr("Google Authenticator")
            }
            ProgressIndicator {
                horizontalAlignment: HorizontalAlignment.Fill
                fromValue: 0
                toValue: 29
                value: _app.remain
            }
            ListView {
                id: listView
                dataModel: _app.dataModel
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill

                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container {
                            layout: DockLayout {
                            }
                            Divider {
                                opacity: 1
                            }
                            Container {
                                Label {
                                    text: ListItemData.email
                                    textStyle.fontSize: FontSize.Small
                                    textStyle.color: Color.Blue
                                }
                                Label {
                                    text: ListItemData.code
                                    textStyle.fontSize: FontSize.XLarge
                                }
                            }
                            ImageButton {
                                horizontalAlignment: HorizontalAlignment.Right
                                verticalAlignment: VerticalAlignment.Center
                                defaultImageSource: "asset:///images/refresh.png"
                                pressedImageSource: "asset:///icons/ic_rotate.png"
                                disabledImageSource: "asset:///icons/ic_rotate.png"
                                visible: ! ListItemData.type
                                enabled: ListItemData.enabled
                                onClicked: {
                                    
                                }
                            }
                            onCreationCompleted: {
                                console.log("Account Test")
                                console.log("Email: " + ListItemData.email)
                                console.log("Code: " + ListItemData.code)
                            }
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Authenticator Code")
                                    ActionItem {
                                        title: qsTr("Copy to Clipboard")
                                        imageSource: "asset:///icons/ic_copy.png"
                                        onTriggered: {
                                        	
                                        }
                                    }
                                    ActionItem {
                                        title: qsTr("Delete This Entry")
                                        imageSource: "asset:///icons/ic_delete.png"
                                        onTriggered: {
                                            Qt.dlg.body = qsTr("Are you sure to delete account: %0").arg(ListItemData.email)
                                            Qt.dlg.show()
                                        }
                                    }
                                }
                            ]
                        }
                    }
                ]
                property variant activeItem: undefined
                onActivationChanged: {
                    if (active) activeItem = indexPath; //[0]
                }
            }
        }
    }
    onCreationCompleted: {
        // this slot is called when declarative scene is created
        // write post creation initialization here
        console.debug("Page - onCreationCompleted()")
        Qt.page = page;
        Qt.app = _app;
        Qt.dlg = confirmDialog;
        // _app.log("Page - onCreationCompleted()")
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
