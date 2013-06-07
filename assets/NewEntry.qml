import bb.cascades 1.0

Page {
    signal done()
    titleBar: TitleBar {
        acceptAction: ActionItem {
            title: qsTr("Create")
            onTriggered: {
                _app.add(account.text, key.text, digit.selectedValue, type.selectedIndex)
                done()
            }
        }
        dismissAction: ActionItem {
            title: qsTr("Cancel")
            onTriggered: {
                done()
            }
        }
        title: qsTr("New Key Entry")
    }
//    actions: [
//        ActionItem {
//            title: qsTr("Clear")
//            ActionBar.placement: ActionBarPlacement.OnBar
//            imageSource: "asset:///icons/ic_cancel.png"
//            onTriggered: {
//                reset();
//            }
//        }
//    ]
    function reset(){
        account.resetText()
        key.resetText()
        digit.selectedIndex = 0
        type.selectedIndex = 0
    }
    Container {
        leftPadding: 20
        rightPadding: 20
        layout: DockLayout {
        }
        Container {
            layout: StackLayout {
            }
            Label {
                text: qsTr("Enter your secret key from the 2-step verification setup page.")
                multiline: true
                textStyle {
                    textAlign: TextAlign.Justify
                }
            }
            Label {

            }
            TextField {
                id: account
                inputMode: TextFieldInputMode.EmailAddress
                input.submitKey: SubmitKey.Next
                hintText: qsTr("Your Account's name")
            }
            Label {
            }
            TextField {
                id: key
                hintText: qsTr("Your Key")
                clearButtonVisible: true
                input.flags: TextInputFlag.AutoCapitalizationOff | TextInputFlag.AutoCorrectionOff | TextInputFlag.AutoPeriodOff | TextInputFlag.PredictionOff | TextInputFlag.SpellCheckOff | TextInputFlag.WordSubstitutionOff
                input.submitKey: SubmitKey.Next
            }
            Label {
            }
            DropDown {
                id: digit
                title: qsTr("Key Length:")
                options: [
                    Option {
                        text: "6"
                        value: 6
                        selected: true
                    },
                    Option {
                        text: "7"
                        value: 7
                    },
                    Option {
                        text: "8"
                        value: 8
                    },
                    Option {
                        text: "9"
                        value: 9
                    }
                ]
            }
            Container {
                layout: DockLayout {
                    // orientation: LayoutOrientation.LeftToRight
                }
                Divider {
                    opacity: 0
                }
                DropDown {
                    id: type
                    title: qsTr("Type:")
                    horizontalAlignment: HorizontalAlignment.Center
                    options: [
                        Option {
                            text: qsTr("Time based OTP")
                            description: qsTr("Time based OTP")
                            selected: true
                        },
                        Option {
                            text: qsTr("Counter based OTP")
                            description: qsTr("Counter based OTP")
                        }
                    ]
                }
            }
        }
    }
}
