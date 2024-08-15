var host = window.location.hostname;

// JSON Scheme for form generation
var drivingSystemSettings = {
    steeringType: {
        value: "Differential Thrust",
        type: "string",
        title: "Steering Type",
        enum: [
            "Rudder",
            "Differential Thrust"
        ]
    },
    motor1_driver: {
        value: "H-Bridge",
        type: "string",
        title: "Motor 1 Driver",
        enum: [
            "ESC",
            "H-Bridge"
        ]
    },
    motor1_pinA: {
        value: 0,
        type: "number",
        title: "Motor 1 Pin A"
    },
    motor1_pinB: {
        value: 0,
        type: "number",
        title: "Motor 1 Pin B"
    },
    motor1_pinE: {
        value: 0,
        type: "number",
        title: "Motor 1 Enable Pin"
    },
    motor2_driver: {
        value: "H-Bridge",
        type: "string",
        title: "Motor 2 Driver",
        enum: [
            "ESC",
            "H-Bridge"
        ]
    },
    motor2_pinA: {
        value: 0,
        type: "number",
        title: "Motor 2 Pin A"
    },
    motor2_pinB: {
        value: 0,
        type: "number",
        title: "Motor 2 Pin B"
    },
    motor2_pinE: {
        value: 0,
        type: "number",
        title: "Motor 2 Enable Pin"
    },
    motor1_max: {
        value: 100,
        type: "number",
        title: "Motor 1 Max"
    },
    motor1_min: {
        value: 0,
        type: "number",
        title: "Motor 1 Min"
    },
    motor1_center: {
        value: 50,
        type: "number",
        title: "Motor 1 Center"
    },
    motor2_max: {
        value: 100,
        type: "number",
        title: "Motor 2 Max"
    },
    motor2_min: {
        value: 0,
        type: "number",
        title: "Motor 2 Min"
    },
    motor2_center: {
        value: 50,
        type: "number",
        title: "Motor 2 Center"
    }
};

var wifiSettings = {
    ap_ssid: {
        value: "BoatyMcBoatface",
        type: "string",
        title: "SSID"
    },
    ap_password: {
        value: "ImTheCaptainNow",
        type: "string",
        title: "Password"
    },
    sta_ssid: {
        value: "Network Name",
        type: "string",
        title: "SSID"
    },
    sta_password: {
        value: "",
        type: "string",
        title: "Password"
    }

};
//var host = 

function init() {
    console.log("settings.js");
    generateDriveSystemForm();
    setupWifiForm();
    setupLedForm();
    document.getElementById("send-drive-system-settings").addEventListener("click", sendDriveSystemSettings);

    getJson("http://" + host + "/settings/drivingsystem","driving-system-form");
    getJson("http://" + host + "/settings/wifi","wifi-form");
    getJson("http://" + host + "/settings/leds","led-form");
}

init();

function setupLedForm() {
    document.getElementById("led-form").onsubmit = function (event) {
        event.preventDefault();
        console.log("LED form submitted");
        sendLedSettings();
    }
}

function sendLedSettings() {
    const url = "http://" + host + "/settings/leds";
    const payload = formToJson("led-form");
    sendJson(url, payload,"led-form");
}

// Form generation/ setup
function setupWifiForm() {
    document.getElementById("wifi-form").onsubmit = function (event) {
        event.preventDefault();
        console.log("Form submitted");
        sendWifiSettings();
    }

    updateFromFromJson("wifi-form",wifiSettings,true);
}

function generateDriveSystemForm() {
    console.log("Generating drive system form");
    const form = document.getElementById("driving-system-form");

    for (const key in drivingSystemSettings) {
        const setting = drivingSystemSettings[key];
        const linebreak = document.createElement("br");
        const label = document.createElement("label");
        label.textContent = setting.title || key;
        label.htmlFor = key;
        form.appendChild(label);

        if (setting.enum) {
            const select = document.createElement("select");
            select.name = key;
            select.id = key;

            setting.enum.forEach(option => {
                const optionElement = document.createElement("option");
                optionElement.value = option;
                optionElement.textContent = option;
                select.appendChild(optionElement);
            });

            form.appendChild(select);
        }
        else {
            const input = document.createElement("input");
            input.id = key;
            input.name = key;
            input.type = setting.type;
            input.value = setting.value;
            form.appendChild(input);
        }

        form.appendChild(linebreak);
    }

    form.onsubmit = function (event) {
        event.preventDefault();
        console.log("Driving System submitted");
        sendDriveSystemSettings();
    }

    const submitButton = document.createElement("button");
    submitButton.type = "submit";
    submitButton.id = "send-drive-system-settings";
    submitButton.textContent = "Save";
    form.appendChild(submitButton);
}

// Form to JSON trnaslators
function formToJson(formId) {
    const form = document.getElementById(formId);
    const formData = new FormData(form);
    var json = {};
    
    for (const [key, value] of formData.entries()) {
        json[key] = value;
    }

    return json;
}

function updateFromFromJson(formId,json,sheme=false){
    const form = document.getElementById(formId);
    for (const key in json) {
        const input = form.querySelector(`input[name=${key}]`);
        if (input) {
            if(sheme){ // This is for initial setup using the sheme
                input.value = json[key].value;
            }
            else{ // Server message will be a flat structure
                input.value = json[key];
            }
        }
    }
}

// Communication Functions
function sendDriveSystemSettings() {
    console.log("sendDriveSystemSettings");
    const url = "http://" + host + "/settings/drivingsystem";
    const payload = formToJson("driving-system-form");
    sendJson(url, payload,"driving-system-form");
}

function sendWifiSettings() {
    console.log("sendWifiSettings");
    const url = "http://" + host + "/settings/wifi";
    const payload = formToJson("wifi-form");
    sendJson(url, payload,"wifi-form");
}

function getJson(url,formName=null) {
    console.log("getting json");
    fetch(url)
    .then(response => response.json())
    .then(data => {if(formName){updateFromFromJson(formName,data,false);}})
    .catch(error => console.log(error));
}

function sendJson(url, json,formName=null) {
    console.log("sendJson");
    console.log(json);
    fetch(url, {
        method: "POST",
        body: JSON.stringify(json),
        headers: {
            "Content-Type": "application/json"
        }
    })
    .then(response => response.json())
    .then(data => {if(formName){updateFromFromJson(formName,data,false);}})
    .catch(error => console.log(error));
}