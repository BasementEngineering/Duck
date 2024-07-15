import './style.css'
import * as THREE from 'three'
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js'
import { FontLoader } from 'three/addons/loaders/FontLoader.js'
import { TextGeometry } from 'three/addons/geometries/TextGeometry.js'
import * as esp from 'esp-web-tools/dist/web/install-button.js?module'

// Asset imports
import icon from '/favicon.png?url'
import duckModel from '/3d_models/Duck2.glb?url'
import arialFont  from '/fonts/Arial_Bold.json?url'

import firmwareManifest from '/firmware/manifest.json?url'

// Create a scene
const scene = new THREE.Scene();

// Create a camera
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
camera.position.set(0,1.7,3);
camera.lookAt(scene.position);

// Create a renderer
const renderer = new THREE.WebGLRenderer();
renderer.setClearColor(new THREE.Color("#ffdf4f"));
renderer.setSize(window.innerWidth, window.innerHeight);
container.appendChild(renderer.domElement);

// Add Text
var font = null;
var mesh = null;

function createText(){
    var text = "Flash Your Duck !";
    const textGeom = new TextGeometry(text, {
        font: font,
        size: 0.2,
        height: 0.05,
        curveSegments: 12,
        bevelEnabled: false
    });
    mesh = new THREE.Mesh(textGeom, new THREE.MeshBasicMaterial({color: 0x000000}));
    mesh.position.set(-1.0,1.5,0);
    scene.add(mesh);
}

function loadFont() {
    var loader = new FontLoader();
    loader.load(arialFont, function (res) {
    font = res;
    createText();
    });
}

var model = null;
function setupModel(){
    const loader = new GLTFLoader();
    loader.load(duckModel, function (gltf) {
    model = gltf.scene;
    model.scale.set(2,2,2);
    model.position.set(0,-0.4,0);
    scene.add(model);
});
}

//document.addEventListener('mousemove', function(event) 
// Make the model always face the cursor
function adjustModelToCursor(event)
{
    // Calculate the mouse position relative to the window
    const mouse = {
        x: (event.clientX / window.innerWidth) * 2 - 1,
        y: -(event.clientY / window.innerHeight) * 2 + 1
    };

    // Calculate the mouse position in 3D space
    const vector = new THREE.Vector3(mouse.x, mouse.y, 0.5);
    vector.unproject(camera);

    // Calculate the direction from the camera to the mouse position
    const direction = vector.sub(camera.position).normalize();

    // Calculate the distance from the camera to the plane containing the model
    const distance = -camera.position.z / direction.z;

    // Calculate the position of the mouse on the plane containing the model
    const position = camera.position.clone().add(direction.multiplyScalar(distance));

    // Calculate the direction from the model to the mouse position
    const target = position.sub(model.position).normalize();

    // Set the rotation of the model to face the mouse position
    model.quaternion.setFromUnitVectors(new THREE.Vector3(0, 0, 1), target);
};

function setupLights(){
    // Add lighting
    const ambientLight = new THREE.AmbientLight(0xffffff, 1.0);
    scene.add(ambientLight);

    const light = new THREE.HemisphereLight( 0xffffbb, 0x080820, 0.75 );
    scene.add( light );

    const directionalLight = new THREE.DirectionalLight(0xffffff, 2.0);
    directionalLight.position.set(0, 0, 1);
    scene.add(directionalLight);
    //const helper = new THREE.DirectionalLightHelper( directionalLight );
    //scene.add( helper );
}

// Animation loop
function animate() {
    requestAnimationFrame(animate);

    // Rotate the model
    if (model) {
        model.rotation.y += 0.01;
    }

    renderer.render(scene, camera);
}

// Add the flash button
const flashButton = document.createElement('esp-web-install-button');
flashButton.id = 'flashButton';
flashButton.setAttribute('manifest', firmwareManifest);
document.getElementById("buttonContainer").appendChild(flashButton);

setupModel();
loadFont();
setupLights();
animate();