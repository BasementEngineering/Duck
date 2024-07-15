import { initUi,showErrorMessage,hideErrorMessage, showPopupMenu,highlightModeButton } from "./my_modules/ui";
import { CommunicationManager } from "./my_modules/backendCommunication";
import { Communication_Commands } from "./my_modules/parser";

import "bootstrap-icons/font/bootstrap-icons.css";

const myCommunicationManager = new CommunicationManager;

function init(){
    initUi();
    document.getElementById("LedSendButton").onclick = sendLedData;
    myCommunicationManager.setStatusCallback(onStatusUpdate);
    myCommunicationManager.setOnlineCallback(onOnline);
    myCommunicationManager.setOfflineCallback(onOffline);

    setTimeout(function(){
        console.log("Attempting to connect");
        myCommunicationManager.connect();
        startReconnection();
        setInterval(function(){
            myCommunicationManager.updateControls();
            myCommunicationManager.sendHeartbeat();
          }, 100);

    },2000);
}

Window.onload = init();

var reconnectionTimer = -1;
var prevConnectionState = true;

function onOffline(){
    console.log("Went offline");
    startReconnection();
    showErrorMessage();
    leftJoystick.reset();
    rightJoystick.reset();
}

function onOnline(){
    console.log("Went back online");
    clearInterval(reconnectionTimer);
    reconnectionTimer = -1;
    hideErrorMessage();
}

function startReconnection(){
    if(reconnectionTimer == -1){
        console.log("Setting reconnection timer");
        reconnectionTimer = setInterval(() => {
            console.log("Attempting reconnection");
            myCommunicationManager.reconnect();
        }, 5000);
    }
}

function onStatusUpdate(command){
    var batteryPercentage = parseInt(command.parameters[0]);
    var networkPercentage = parseInt(command.parameters[1]);
    percentageToIcon(batteryPercentage,"Battery");
    percentageToIcon(networkPercentage,"Network");
}

function updateControls(){
	if( (leftJoystick!= null) && (rightJoystick!= null) ){
		var command = myCommunicationManagergenerateEmptyCommand();

		if(mode == 1 || mode == 2){	
			command.id = Communication_Commands.ControlSD;
		}
		else if( (mode == 3) || (mode == 4) )
		{
			command.id = Communication_Commands.ControlLR;
		}
		command.parameterCount = 2;
		command.parameters.push(leftJoystick.getPercentage()); //steering
		command.parameters.push(rightJoystick.getPercentage());
		myCommunicationManagersendCommand(command);	
	}
}

function sendLedData(){
    var ledMode = document.getElementById("LedModeSelect").value;
    var hexColor = document.getElementById("LedColorPicker").value;
    console.log(hexColor);
    var rgbColor = hexToRgb(hexColor);

    var command = myCommunicationManager.parser.generateEmptyCommand();
    command.id = Communication_Commands.ControlLed;
    command.parameterCount = 4;
    command.parameters.push(ledMode);
    command.parameters.push(rgbColor.r);
    command.parameters.push(rgbColor.g);
    command.parameters.push(rgbColor.b);

    myCommunicationManager.sendCommand(command);	
  }

  function hexToRgb(hex) {
    var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    return result ? {
      r: parseInt(result[1], 16),
      g: parseInt(result[2], 16),
      b: parseInt(result[3], 16)
    } : null;
  }

  
