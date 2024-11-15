import os, shutil

source_path = os.path.expanduser("~") + "/source"
compounds_path = "A:/TeamEnvironment/Compounds"
source_folder = "Hyuu"
lib_name = "Hyuu"


def removeFolder(rel_path):
    root_path = f"{source_path}/{rel_path}"
    if not os.path.isdir(root_path):
        return

    for f in os.listdir(root_path):
        p = os.path.join(root_path, f)
        if (f.startswith(source_folder) or f.startswith(lib_name)) and os.path.isdir(p):
            shutil.rmtree(p)
            return


# remove previous build
shutil.rmtree(f"{source_path}/repos/HeydoubleU/{source_folder}/out", ignore_errors=True)
removeFolder("bifrost_libs")
removeFolder("builds")

# run batch file
os.system(f"{source_path}/repos/HeydoubleU/{source_folder}/build.bat")

# copy files
for folder in os.listdir(f"{source_path}/bifrost_libs"):
    if not folder.startswith(lib_name):
        continue

    src = f"{source_path}/bifrost_libs/{folder}"
    shutil.copytree(src, f"{compounds_path}/{lib_name}", dirs_exist_ok=True)
    break


# Hyuu Library specific post processing
import json

with open(f"{compounds_path}/Hyuu/lib/OpenCL.json", "r") as f:
    data = json.load(f)
for operator in data["operators"]:
    if operator["name"] != "Hyuu::OpenCL::Execute::set_kernel_arg":
        continue
    metadata = operator.setdefault("metadata", [])
    metadata.append({
        "metaName": "NodeValueDisplay",
        "metadata": [
            {
                "metaName": "show",
                "metaType": "string",
                "metaValue": "1"
            },
            {
                "metaName": "format",
                "metaType": "string",
                "metaValue": "Set Kernel Arg {arg_id}"
            }
        ]
    })

with open(f"{compounds_path}/Hyuu/lib/OpenCL.json", "w") as f:
    json.dump(data, f, indent=4)
