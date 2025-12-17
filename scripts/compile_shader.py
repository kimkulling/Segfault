import subprocess
import argparse
import shutil
import os

from pathlib import Path
from os import listdir
from os.path import isfile, join

def compile_shader(shadername, shader_out, verbose):
    if len(shadername) == 0:
        return 

    cmd = []
    cmd.append("glslc")
    cmd.append(shadername)
    cmd.append("-o")
    cmd.append(shader_out)
    process_handle = subprocess.Popen(cmd, stdout=subprocess.PIPE)
    output, error = process_handle.communicate()
    if error is None:
        print("Shader " + shadername + " compiled.")
        if verbose and output is not None:
            if len(output) != 0:
                print(str(output))
    else:
        print("Error {error} while compilation.", error)

def copy_shader(source, dest):
    if not os.path.exists(dest):
        print("Create folder " + dest)
        os.makedirs(dest)
    print("source " + source)
    shutil.copy(source, dest)

shader_names = ["default.vert", "default.frag"]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--verbose', action='store_true', default=False, help='The full output will be shown')
    parser.add_argument('--shader', type=str, required=True, default='./', help='The folder containing the shaders')
    
    args = parser.parse_args()
    print("shader folder: " + str(args.shader))
    shader_files = [f for f in listdir(args.shader) if isfile(join(args.shader, f))]
    print("shader files = " + str(shader_files))
    for shader in shader_files:
        if shader in shader_names:
            path = Path(shader)
            shader_out = path.suffix[1:len(path.suffix)] + ".spv"
            compile_shader(args.shader + shader, shader_out, args.verbose)

            copy_shader(shader_out, "../bin/debug/shaders")

if __name__=="__main__":
    main()


