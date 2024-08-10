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

// Form generation/ setup
function setupWifiForm() {
    document.getElementById("wifi-form").onsubmit = function (event) {
        event.preventDefault();
        console.log("Form submitted");
        var myForm = document.getElementById("wifi-form");
        console.log(myForm);
        var formData = new FormData(myForm);
        console.log(formData.entries());

        // Display the key/value pairs
        for (const pair of formData.entries()) {
            console.log(pair[0], pair[1]);
        }
    }
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

// Sending functions
function sendJson(url, json) {
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
    .then(data => console.log(data))
    .catch(error => console.log(error));
}

function drivingSystemSettingsFormToJson() {
    console.log("drivingSystemSettingsFormToJson");
    const form = document.getElementById("driving-system-form");
    const formData = new FormData(form);
    var json = {};

    for (const [key, value] of formData.entries()) {
        json[key] = value;
    }

    return json;
}

function sendDriveSystemSettings() {
    console.log("sendDriveSystemSettings");
    const url = "http://" + window.location.hostname + "/settings/drive_system";
    const payload = drivingSystemSettingsFormToJson();
    sendJson(url, payload);
}

function sendWifiSettings() {
    console.log("sendWifiSettings");
    const url = "http://" + window.location.hostname + "/settings/wifi";
    const payload = wifiSettingsFormToJson();
    sendJson(url, payload);
}

function init() {
    console.log("settings.js");
    generateDriveSystemForm();
    setupWifiForm();
    document.getElementById("send-drive-system-settings").addEventListener("click", sendDriveSystemSettings);
}

init();

