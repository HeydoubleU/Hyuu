# Add Documentation metadata and create .md doc file for all compounds in a directory
# (that don't already it)

import os
import json

def hasCompounds(data):
    return "compounds" in data

def removeExistingDoc(compound):
    for item in compound["metadata"]:
        if item["metaName"] == "documentation":
            compound["metadata"].remove(item)
            return
        
def removeExistingIcon(compound):
    for item in compound["metadata"]:
        if item["metaName"] == "icon":
            compound["metadata"].remove(item)
            return

ROOT = "C:/Users/MAIN/Autodesk/Bifrost/Compounds/Team/Hyuu"

for root, dirs, files in os.walk(ROOT):
    if not "Signal" in root:
        continue

    for file in files:
        path = os.path.join(root, file).replace("\\", "/")

        if not path.endswith(".json"):
            continue
        with open(path, "r") as f:
            data = json.load(f)
        if not hasCompounds(data):
            continue

        rel_path = path.replace(ROOT, "")[1:]
        num_subfolders = rel_path.count("/")
        doc_root = "../" * num_subfolders + "documentation/" + rel_path
        icon_root = "../" * num_subfolders + "icons/"
        doc_root = os.path.dirname(doc_root)

        for compound in data["compounds"]:
            removeExistingDoc(compound)
            removeExistingIcon(compound)
            comp_name = compound["name"].split("::")[-1]
            is_opencl = compound["name"].startswith("Hyuu::OpenCL::")

            compound["metadata"].append({
                "metaName": "documentation",
                "metaType": "string",
                "metaValue": f"{doc_root}/{comp_name}.md"
            })

            compound["metadata"].append({
                "metaName": "icon",
                "metaType": "string",
                "metaValue": icon_root + ("opencl_icon.png" if is_opencl else "signal_icon.png")
            })

            doc_filename = f"{ROOT}/documentation/{os.path.dirname(rel_path)}/{comp_name}.md"
            os.makedirs(os.path.dirname(doc_filename), exist_ok=True)
            if not os.path.isfile(doc_filename):
                with open(doc_filename, "w") as f:
                    f.write(f"document me")
        
        with open(path, "w") as f:
            json.dump(data, f, indent=4)
        

