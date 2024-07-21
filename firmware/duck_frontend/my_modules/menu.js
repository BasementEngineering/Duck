export class Menu {
    constructor() {
        this.menuItems = [
            {id: 0, name: "Lights", screen: "Lights"},
            {id: 1, name: "Motors", screen: "Motors"},
            {id: 2, name: "WiFi", screen: "WiFi"},
            {id: 3, name: "Input", screen: "Input"}
        ];
        this.currentMenu = this.menuItems[0];
        this.active = false;
    }

    init() {
        document.getElementById("SettingsButton").onclick = this.showPopupMenu;
        document.getElementById("SettingsBackButton").onclick = this.hidePopupMenu;

        this.setupSettingsNavigation();
    }

    setupSettingsNavigation() {
        let settingsNavigation = document.getElementById("settingsNavigation");

        this.menuItems.forEach(item => {
            let menuItem = document.createElement('button');
            menuItem.id = "MenuItem"+item.id+"Button";
            menuItem.innerHTML = item.name;
            menuItem.onclick = () => {
                this.currentMenu = item;
                this.highlightButton("MenuItem",item.id,this.menuItems.length);
                this.renderScreen();
            };
            settingsNavigation.appendChild(menuItem);
        });
        this.highlightButton("MenuItem",this.menuItems[0].id,this.menuItems.length);
        this.renderScreen();
    }

    /*renderMotorPinInputs(){
        let screenArea = document.getElementById("settingsPage");
        var motorDriverChoice = document.getElementById("MotorDriverType").value;

        let header1 = document.createElement("h3");	
        header1.innerHTML = "Motor 1";
        
        motorPin1.type = "number";
        motorPin1.id = "MotorPin1";
        motorPin1.value = "0";
        document.getElementById("settingsPage").appendChild(header1);
        document.getElementById("settingsPage").appendChild(motorPin1);

        if(motorDriverChoice == "H-Bridge"){
                let motorPin2 = document.createElement("input");
                motorPin2.type = "number";
                motorPin2.id = "MotorPin2";
                motorPin2.value = "0";
                let header2 = document.createElement("h3");
        }

        let motorPin1 = document.createElement("input");
        motorPin1.type = "number";
        motorPin1.id = "MotorPin1";
        motorPin1.value = "0";

        let motorPin2 = document.createElement("input");
        motorPin2.type = "number";
        motorPin2.id = "MotorPin2";
        motorPin2.value = "0";

        return [motorPin1,motorPin2];
    }*/

    appendPinSetting(form, pinName, parameterPrefix){
        let inputDiv1 = document.createElement("div");
        let inputLabel1 = document.createElement("label");
        inputLabel1.innerHTML = pinName;
        let motorPin1 = document.createElement("input");
        motorPin1.type = "number";
        motorPin1.id = parameterPrefix;
        motorPin1.value = "0";
        inputDiv1.appendChild(inputLabel1);
        inputDiv1.appendChild(motorPin1);
        form.appendChild(inputDiv1);
    }

    appendMotorSettings(form, motorName,parameterPrefix){
        let header = document.createElement("h3");
        header.innerHTML = motorName;
        form.appendChild(header);

        // Motor Driver Type Select
        let motorDriverTypeDiv = document.createElement("div");
        let motorDriverTypeLabel = document.createElement("label");
        motorDriverTypeLabel.for = "MotorDriverType";
        motorDriverTypeLabel.innerHTML = "Motor Driver Type";
        let motorDriverType = document.createElement("select");
        motorDriverType.id = "MotorDriverType";
        motorDriverType.innerHTML = '<option value="0">H-Bridge</option><option value="1">ESC</option>';
        motorDriverTypeDiv.appendChild(motorDriverTypeLabel);
        motorDriverTypeDiv.appendChild(motorDriverType);
        form.appendChild(motorDriverTypeDiv);

        if(motorDriverType.value == "0"){
            this.appendPinSetting(form, "Pin 1", parameterPrefix+"pin1");
            this.appendPinSetting(form, "Pin 2", parameterPrefix+"pin2");
            this.appendPinSetting(form, "Enable Pin", parameterPrefix+"en");
        }
        else if(motorDriverType.value == "1"){
            this.appendPinSetting(form, "Pin", parameterPrefix+"pin1");
        }
    }
        

    renderMotorsScreen(){
        let parameterPrefix = "ds_";

        let screenArea = document.getElementById("settingsPage");
        screenArea.innerHTML = "";

        let form = document.createElement("form");
        form.id = "Drive System Settings";
        form.action = "/settings/drive_system";
        form.method = "post";

        // Drive System Type Select
        let driveSystemTypeDiv = document.createElement("div");
        let driveSystemTypeLabel = document.createElement("label");
        driveSystemTypeLabel.for = "driveSystemType";
        driveSystemTypeLabel.innerHTML = "Drive System Type";
        let driveSystemType = document.createElement("select");
        driveSystemType.id = "driveSystemType";
        driveSystemType.name = parameterPrefix+"type";
        driveSystemType.innerHTML = '<option value="0">H-Bridge</option><option value="1">ESC</option>';
        driveSystemTypeDiv.appendChild(driveSystemTypeLabel);
        driveSystemTypeDiv.appendChild(driveSystemType);
        form.appendChild(driveSystemTypeDiv);

        // Motor 1
        this.appendMotorSettings(form, "Left Motor", parameterPrefix+"motor1_");

        // Submit Button
        let submitButton = document.createElement("button");
        submitButton.type = "submit";
        submitButton.innerHTML = "Save";
        form.appendChild(submitButton);

        screenArea.appendChild(form);
    }

    renderWifiScreen() {
        let screenArea = document.getElementById("settingsPage");
        screenArea.innerHTML = "";

        let ssidInput = document.createElement("input");
        ssidInput.type = "text";
        ssidInput.id = "ssidInput";
        ssidInput.placeholder = "SSID";

        let passwordInput = document.createElement("input");
        passwordInput.type = "password";
        passwordInput.id = "passwordInput";
        passwordInput.placeholder = "Password";

        let sendButton = document.createElement("button");
        sendButton.id = "WifiSendButton";
        sendButton.innerHTML = "Save";

        screenArea.appendChild(ssidInput);
        screenArea.appendChild(passwordInput);
        screenArea.appendChild(sendButton);
    }

    renderLightsScreen() {
        let screenArea = document.getElementById("settingsPage");
        screenArea.innerHTML = "";

        let select = document.createElement("select");
        select.id = "LedModeSelect";
        select.innerHTML = '<option value="0">Solid Color</option><option value="1" selected="selected">Knight Rider</option><option value="2">Blinking</option><option value="3">Boat</option>';

        let sendButton = document.createElement("button");
        sendButton.id = "LedSendButton";
        sendButton.innerHTML = "Update LEDs";
        
        let colorPicker = document.createElement("input");
        colorPicker.type = "color";
        colorPicker.id = "LedColorPicker";
        colorPicker.value = "#fcca03";

        screenArea.appendChild(select);
        screenArea.appendChild(sendButton);
        screenArea.appendChild(colorPicker);
    }


    renderScreen() {
        switch(this.currentMenu.screen) {
            case "Lights":
                this.renderLightsScreen();
                break;
            case "Motors":
                this.renderMotorsScreen();
                break;
            case "WiFi":
                this.renderWifiScreen();
                break;
            case "Input":
                this.renderInputScreen();
                break;
        }
    }

    showPopupMenu() {
        document.getElementById("popupMenu").style.visibility = "visible";
        this.active = true;
    }

    hidePopupMenu() {
        document.getElementById("popupMenu").style.visibility = "hidden";
        this.active = false;
    }

    highlightButton(idPrefix,choosenOption,optionCount){ 
        for(var i = 0; i < optionCount; i++){
            var elementId = idPrefix+i+"Button";
            document.getElementById(elementId).style.borderColor = (i==choosenOption) ? "rgb(255, 204, 0)" : "rgb(179, 178, 175)";
        }
    }
/*
    highlightModeButton(newMode){
        for(var i = 1; i <= 4; i++){
          document.getElementById("Mode"+i+"Button").style.borderColor = (i==newMode) ? "rgb(255, 204, 0)" : "rgb(179, 178, 175)";
        }
    }*/
}

export default Menu;

/*
LED Settings
<div id="LedPanel">
  <div id="LedControlsGrid">
      <div id="Spacer" ></div>
      <select id="LedModeSelect" >
          <option value="0">Solid Color</option>
          <option value="1" selected="selected">Knight Rider</option>
          <option value="2">Blinking</option>
          <option value="3">Boat</option>
        </select>
        <button type="button" id="LedSendButton">Update LEDs</button>
      <input type="color" id="LedColorPicker" value="#fcca03">
  </div>
</div>
<div id="Motor Settings">
<select id="Motor Driver Type" >
  <option value="0">H-Bridge</option>
  <option value="1">ESC</option>
</select>
<input type="number" id="MotorPin1" value="0">
<input type="number" id="MotorPin2" value="0">

</div>
Input Settings
<div id="ModePanel">
<button type="button" id="Mode1Button">Mode 1</button>
<button type="button" id="Mode2Button">Mode 2</button>
<button type="button" id="Mode3Button">Mode 3</button>
<button type="button" id="Mode4Button">Mode 4</button>
</div></input>v*/