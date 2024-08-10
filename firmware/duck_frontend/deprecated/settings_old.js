import { JSONEditor } from "@json-editor/json-editor";

var ledEditor = null;
var wifiEditor = null;
var driveSystemEditor = null;

setuptEditor();

function setuptEditor() {
    const ledContainer = document.getElementById("led-editor");
    const wifiContainer = document.getElementById("wifi-editor");
    const driveSystemContainer = document.getElementById("drive-system-editor");

    var options = {
        theme: 'bootstrap4',
        disable_edit_json: true,
        disable_properties: true,
        disable_collapse: true,
        schema: getUniversalDriveSystemSchema()
    };

    console.log(options.schema);

    driveSystemEditor = new JSONEditor(driveSystemContainer, options);

    document.getElementById("send-drive-system-settings").addEventListener("click", sendDriveSystemSettings);
}

function sendDriveSystemSettings() {
    console.log("sendDriveSystemSettings");
    console.log(driveSystemEditor.getValue());
    const url = "http://"+window.location.hostname+"settings/drive_system";
    const payload = driveSystemEditor.getValue();

    fetch(url, {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(payload)
    })
        .then(response => response.json())
        .then(data => {
            console.log("Response:", data);
            // Handle the response data here
        })
        .catch(error => {
            console.error("Error:", error);
            // Handle any errors here
        });
}

// Drive System Functions
function getPinsSchema(title="Pins") {
    return {
        type: "object",
        title: title,
        properties: {
            pin1: {
                type: "integer",
                format: "number",
                default: 12,
                minimum: 0,
                maximum: 50
            },
            pin2: {
                type: "integer",
                format: "number",
                default: 12,
                minimum: 0,
                maximum: 50
            },
            enablePin: {
                type: "integer",
                format: "number",
                default: 12,
                minimum: 0,
                maximum: 50
            }
        }
    }
}

function getUniversalOutputSchema(title="Output") {   
    return {
        type: "object",
        title: title,
        properties: {

            max: {
                type: "number",
                format: "range",
                default: 100,
                minimum: 50,
                maximum: 100
            },
            min: {
                type: "number",
                format: "range",
                default: 0,
                minimum: 0,
                maximum: 50
            },
            center: {
                type: "number",
                format: "range",
                default: 50,
                minimum: 0,
                maximum: 100
            }
        }
    }
}

function getUniversalMotorSchema(title="Motor") {
    return {
        type: "object",
        title: title,

        properties: {

            motor_driver: {
                type: "string",
                enum: [
                    "ESC",
                    "H-Bridge"
                ]
            },
            pins: getPinsSchema("Pins"),
            output: getUniversalOutputSchema("Output")
        }
    }
}

function getUniversalDriveSystemSchema(){
    console.log('getUniversalDriveSystemSchema');
    return {
        type: "object",
        title: "Drive System",
        properties: {
            steeringType: {
                type: "string",
                enum: [
                    "Rudder",
                    "Differential Thrust",
                ]
            },
            motor_1: getUniversalMotorSchema("Motor 1"),
            motor_2: getUniversalMotorSchema("Motor 2")
        }
    }
}

function getDefaultDriveSystemValues(){
    return (
            {
                "steeringType": "Differential Thrust",
                "motor_1": {
                    "motor_driver": "H-Bridge",
                    "pins": {
                        "pin1": 13,
                        "pin2": 12,
                        "enablePin": 15
                    },
                    "output": {
                        "max": 100,
                        "min": 20,
                        "center": 50
                    }
                },
                "motor_2": {
                    "motor_driver": "H-Bridge",
                    "pins": {
                        "pin1": 14,
                        "pin2": 0,
                        "enablePin": 15
                    },
                    "output": {
                        "max": 100,
                        "min": 20,
                        "center": 50
                    }
                }
            }
        );
    }
