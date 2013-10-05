// Application with the Sheet project template
import bb.cascades 1.0
import bb.system 1.0

Page {
    id: page
    actions: [
        ActionItem {
            title: qsTr("Add account")
            imageSource: "asset:///icons/ic_add.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: [
                SystemShortcut {
                    type: SystemShortcuts.CreateNew
                },
                Shortcut {
                    key: "N"
                }
            ]
            onTriggered: {
                sheetNew.open()
            }
        },
        ActionItem {
            title: qsTr("Scan QR Barcode")
            imageSource: "asset:///icons/ic_scan_barcode.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: Shortcut {
                key: "S"
            }
            onTriggered: {
                _app.scanBarcode()                
            }
        },
        InvokeActionItem {
            title: qsTr("Share")
            ActionBar.placement: ActionBarPlacement.InOverflow
            query {
                mimeType: "text/plain"
                uri: "https://appworld.blackberry.com/webstore/content/29401059/"
                data: qsTr("Check out 2 Steps Authenticator in BlackBerry World! You can find it at https://appworld.blackberry.com/webstore/content/29401059/")
                invokeActionId: "bb.action.SHARE"
            }
        },
        InvokeActionItem {
            title: qsTr("Buy me a beer")
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///icons/ic_donate.png"
            query{
                mimeType: "text/plain"
                uri: "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=6XZ562VGST6H2"
                invokeActionId: "bb.action.OPEN"
            }
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
                text: qsTr("2 Steps Authenticator")
            }
            ProgressIndicator {
                horizontalAlignment: HorizontalAlignment.Fill
                fromValue: 0
                toValue: 300
                value: _app.elapsed
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
                                minWidth: 720
                                Label {
                                    text: ListItemData.email
                                    textStyle.fontSize: FontSize.Default
                                }
                                Label {
                                    text: ListItemData.code
                                    textStyle.fontSize: FontSize.XLarge
                                }
                                onTouch: {
                                    if (event.isUp()) {
                                        Qt.app.insertToClipboard(ListItemData.code)
                                    }
                                }
                            }
                            ImageButton {
                                horizontalAlignment: HorizontalAlignment.Right
                                verticalAlignment: VerticalAlignment.Center
                                defaultImageSource: "asset:///images/refresh.png"
                                disabledImageSource: "asset:///icons/ic_rotate.png"
                                visible: ListItemData.type
                                enabled: ListItemData.enabled
                                onClicked: {
                                    var s = ListItemData.next
                                }
                            }
                            onTouchEnter: {   
                                console.debug("Parent Container")                      
                                Qt.app.insertToClipboard(ListItemData.code)
                            }
                            contextActions: [
                                ActionSet {
                                    title: qsTr("Authenticator Code")
                                    subtitle: ListItemData.email
                                    DeleteActionItem {
                                        title: qsTr("Delete This Entry")
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
                
                attachedObjects: [                    
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
        console.debug(qsTr("Generate verification codes for the 2-step verification feature for your Google Account"))
        Qt.page = page;
        Qt.app = _app;
        Qt.dlg = confirmDialog;
        // _app.log("Page - onCreationCompleted()")
        // enable layout to adapt to the device rotation
        // don't forget to enable screen rotation in bar-bescriptor.xml (Application->Orientation->Auto-orient)
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
    }
}
