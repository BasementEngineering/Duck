import os

def padded_hex(i, l):
    given_int = i
    given_len = l

    hex_result = hex(given_int)[2:] # remove '0x' from beginning of str
    num_hex_chars = len(hex_result)
    extra_zeros = '0' * (given_len - num_hex_chars) # may not get used..

    return ('0x' + hex_result if num_hex_chars == given_len else
            '?' * given_len if num_hex_chars > given_len else
            '0x' + extra_zeros + hex_result if num_hex_chars < given_len else
            None)

def to_byte_array_string(content_string):
    #byte_array = [hex(ord(char)) for char in content_string]
    #encoded_string = "{" + ",".join(byte_array) + "}"
    byte_string =""
    length = len(content_string)
    for index,char in enumerate(content_string):
        progress = index / length * 100
        print(progress, end="\r")
        char_code = ord(char)
        if index < length - 1:
            byte_string += f"{padded_hex(char_code,2)},"
        else:
            byte_string += f"{padded_hex(char_code,2)}"

        if char_code > 127:
            print(f"Warning: character '{char}' with code {char_code} is not ASCII")
        if index % 20 == 0:
            byte_string += "\n"
    return byte_string

def file_to_raw_string(file_path: str, filename: str):
    arduino_string = ""
    with open(file_path, 'r', encoding='utf-8',errors='replace') as file:
        content = file.read()

        array_length = len(content)
        encoded_content = to_byte_array_string(content)	
        
        print(f"Encoding {filename} with {array_length} characters")
        arduino_string += f"//File: {filename}\n"
        #"R\"=====(\"{encoded_content})=====\";\n"
        arduino_string += f"const uint16_t {get_artifact_name(filename)}_length = {array_length};\n"
        arduino_string += f"const char {get_artifact_name(filename)}[] PROGMEM = {{ {encoded_content} }};\n"
    
    return arduino_string

def strip_file_extension(filename):
    return os.path.splitext(filename)[0]

def get_artifact_name(file_name: str):
    file_name_parts = file_name.split(".")
    print(file_name)
    print(file_name_parts)
    artifact_name = file_name_parts[0] + "_" + file_name_parts[1]
    if "-" in artifact_name:
        artifact_name = artifact_name.replace("-", "_")
    return artifact_name

def get_mime_string(file_name):
    file_extension = os.path.splitext(file_name)[1]
    mime_type = 'text/plain'

    if file_extension == '.html':
        mime_type = "text/html"
    elif file_extension == '.css':
        mine_type = "text/css"
    elif file_extension == '.js':
        mime_type= "application/javascript"

    return mime_type

def encode_scripts_in_arduino(dist_folder_path: str):
    folder_name = 'scripts'
    folder_path = os.path.join(dist_folder_path, folder_name)
    arduino_string = ""
    arduino_code_header = ""

    for filename in os.listdir(folder_path):
        file_path = os.path.join(folder_path, filename)
        if os.path.isfile(file_path):
            arduino_code_header += get_arduino_function(folder_name,filename)
            arduino_string += file_to_raw_string(file_path, filename)

    return [arduino_code_header,arduino_string]

def encode_icons_in_arduino(dist_folder_path: str):
    folder_name = 'icons'
    folder_path = os.path.join(dist_folder_path, folder_name)
    arduino_string = ""
    arduino_code_header = ""

    for filename in os.listdir(folder_path):
        file_path = os.path.join(folder_path, filename)
        if os.path.isfile(file_path):
            arduino_code_header += get_arduino_function(folder_name,filename)
            arduino_string += file_to_raw_string(file_path, filename)

    return [arduino_code_header,arduino_string]

def get_arduino_function(foldername: str,filename: str):
    mime_type = get_mime_string(filename)
    artifact_name = get_artifact_name(filename)

    path = filename
    if foldername != None:
        path = foldername + "/" + filename

    function_string = f"void handle_{artifact_name}() {{ server.send_P(200, \"{mime_type}\", {artifact_name},{artifact_name}_length);}}\n"
    server_function_string = f"server.on(\"{path}\", handle_{artifact_name});\n"
    return function_string + server_function_string


def encode_index_in_arduino(dist_folder_path: str):
    filename = 'index.html'
    file_path = os.path.join(dist_folder_path, filename)
    arduino_string = ""

    arduino_code_header = get_arduino_function(None,filename)

    arduino_string += file_to_raw_string(file_path, filename)

    return [arduino_code_header,arduino_string]


path = current_file_path = os.path.abspath(__file__)
dist_folder_path = os.path.join(os.path.dirname(current_file_path), 'dist')

index = encode_index_in_arduino(dist_folder_path)
scripts = encode_scripts_in_arduino(dist_folder_path)
icons = encode_icons_in_arduino(dist_folder_path)

output_file_paths = ["",""]
output_file_paths[0] = os.path.join(dist_folder_path, 'website_functions.h')
file_definitions = ["WEBSITE_FUNCTIONS_H","WEBSITE_CONTENT_H"]
output_file_paths[1] = os.path.join(dist_folder_path, 'website_content.h')

print("Writing to file: ", output_file_paths[0])

# ToDo: Add the following code to the main file
#void FrontendServer_init(){
 # server.on("/scripts/dataBuffer.js", handle_dataBuffer_js);
 # server.on("/", handle_index_html);

  #FrontendServer_start();
#}

for i in range(0,2):
    with open(output_file_paths[i], 'w') as file:
        file.write(f"#ifndef {file_definitions[i]}\n")
        file.write(f"#define {file_definitions[i]}\n")
        file.write("\n")
        file.write("#include \"Arduino.h\"\n")
        file.write("\n")
        file.write(index[i])
        file.write(scripts[i])
        file.write(icons[i])
        file.write("\n")
        file.write("#endif\n")
        file.write("\n")