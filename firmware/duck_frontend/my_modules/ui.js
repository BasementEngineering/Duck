import { Joystick } from "./joystick";

var show_errors = false;

export function initUi(){
  scaleItems();
  setupJoysticks();
  document.getElementById("SettingsButton").onclick = showPopupMenu
  document.getElementById("SettingsBackButton").onclick = showPopupMenu
 // drawBatteryIcon();
  setMode(1);
}

function scaleItems(){
  var width = (window.innerWidth > 0) ? window.innerWidth : screen.width;
  console.log("Width: "+width);
  var joystickWidth = width * 0.22;
  setJoystickSize("LeftJoystick",joystickWidth);
  setJoystickSize("RightJoystick",joystickWidth);
}

function setJoystickSize(name,size){
  document.getElementById(name+"Container").style.width = size+20;
  document.getElementById(name+"Container").style.height = size+20
  document.getElementById(name+"Base").width = size;
  document.getElementById(name+"Base").height = size;
  document.getElementById(name).width = size;
  document.getElementById(name).height = size;
}

function setupJoysticks(){
  leftJoystick = new Joystick("LeftJoystick",false);
  rightJoystick = new Joystick("RightJoystick",false);

  document.getElementById("Mode1Button").addEventListener("click", e => setMode(1) );
  document.getElementById("Mode2Button").addEventListener("click", e => setMode(2) );
  document.getElementById("Mode3Button").addEventListener("click", e => setMode(3) );
  document.getElementById("Mode4Button").addEventListener("click", e => setMode(4) );
}

  function toggleVisibility(elementId){
    var value = document.getElementById(elementId).style.visibility;
  
    var newStyle = "visible";
    if(value == "visible"){
      newStyle = "hidden";
    }
  
    document.getElementById(elementId).style.visibility = newStyle;
  }

  //Status Icon Functions
var statusData =[
  {
    name:"Battery",
    indexId: "BatterySymbol",
    icons:["bi bi-battery","bi bi-battery-half","bi bi-battery-full"],
    critical:false
  },
  {
    name:"Network",
    indexId: "NetworkSymbol",
    icons:["bi bi-wifi-1","bi bi-wifi-2","bi bi-wifi"],
    critical:false
  }
];

var statusColors = ["rgb(255, 0, 0)","rgb(255, 153, 51)","rgb(0, 204, 0)"];

function percentageToIcon(percentage,parameterName){
  var newIconName = "";
  var newColor = "";
  var visibility = "";
  var parameterId = statusData.findIndex(x => x.name === parameterName);
  var elementId = statusData[parameterId].indexId;

  if(percentage > 60){
    newIconName = statusData[parameterId].icons[2];
    statusData[parameterId].critical=false;
    visibility = "visible";
    newColor = statusColors[2];
  }
  else if( (percentage > 20) && (percentage <= 60) ){
    newIconName = statusData[parameterId].icons[1];
    statusData[parameterId].critical=false;
    visibility = "visible";
    newColor = statusColors[1];
  }
  else{
    newIconName = statusData[parameterId].icons[0];
    statusData[parameterId].critical=true;
    visibility = document.getElementById(elementId).style.visibility;
    newColor = statusColors[0];
  }
  
  document.getElementById(elementId).className = newIconName;
  document.getElementById(elementId).style.visibility = visibility;
  document.getElementById(elementId).style.color = newColor;
}

function updateStatuIcons(){
  for(var i = 0; i < 2; i++){
    if(statusData[i].critical){
      toggleVisibility(statusData[i].indexId);
    }
  }
}

export function showErrorMessage(){
  if(show_errors){
    document.getElementById("popupError").style.visibility = "visible";
  }
}

export function hideErrorMessage(){
  document.getElementById("popupError").style.visibility = "hidden";
}

//Settings functions
export function showPopupMenu() {
  toggleVisibility("popupMenu")
  }

export function highlightModeButton(newMode){
  for(var i = 1; i <= 4; i++){
    document.getElementById("Mode"+i+"Button").style.borderColor = (i==newMode) ? "rgb(255, 204, 0)" : "rgb(179, 178, 175)";
  }
}

function setMode(newMode){
  mode = newMode;
  highlightModeButton(newMode);

  if(newMode == 1 || newMode == 2){
    leftJoystick.setDeadband(25);
    rightJoystick.setDeadband(0);
    rightJoystick.setRotation(true); //left right steering  
    if(newMode == 1){
      leftJoystick.setStickyness(false);
    }
    else{
      leftJoystick.setStickyness(true);
    }
  }
  else if(newMode == 3 || newMode == 4){
    leftJoystick.setDeadband(25);
    rightJoystick.setDeadband(25);
    rightJoystick.setRotation(false);
    if(newMode == 3){
      leftJoystick.setStickyness(false);
      rightJoystick.setStickyness(false);
    }
    else{
      leftJoystick.setStickyness(true);
      rightJoystick.setStickyness(true);
    }
  }
  
}