#include "Hyuu.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>
#include <unordered_set>

#include <Bifrost/Geometry/DebugDump.h>

/* # python generator script
basetypes = ["float", "double", "char", "uchar", "short", "ushort", "int", "uint", "long", "ulong"]
formats = ["Math::{}2", "Math::{}3", "Math::{}4", "Math::{}2x2", "Math::{}2x3", "Math::{}2x4", "Math::{}3x2", "Math::{}3x3", "Math::{}3x4", "Math::{}4x2", "Math::{}4x3", "Math::{}4x4"]
line = "unordered_set<std::string> NUMERIC_TYPES{"
for bt in basetypes:
    line += '\n    "' + bt + '", '
    for f in formats:
        line += '"' + f.format(bt) + '", '
print(line[:-2] + "\n};")
**/
std::unordered_set<std::string> NUMERIC_TYPES{
    "float", "Math::float2", "Math::float3", "Math::float4", "Math::float2x2", "Math::float2x3", "Math::float2x4", "Math::float3x2", "Math::float3x3", "Math::float3x4", "Math::float4x2", "Math::float4x3", "Math::float4x4",
    "double", "Math::double2", "Math::double3", "Math::double4", "Math::double2x2", "Math::double2x3", "Math::double2x4", "Math::double3x2", "Math::double3x3", "Math::double3x4", "Math::double4x2", "Math::double4x3", "Math::double4x4",
    "char", "Math::char2", "Math::char3", "Math::char4", "Math::char2x2", "Math::char2x3", "Math::char2x4", "Math::char3x2", "Math::char3x3", "Math::char3x4", "Math::char4x2", "Math::char4x3", "Math::char4x4",
    "uchar", "Math::uchar2", "Math::uchar3", "Math::uchar4", "Math::uchar2x2", "Math::uchar2x3", "Math::uchar2x4", "Math::uchar3x2", "Math::uchar3x3", "Math::uchar3x4", "Math::uchar4x2", "Math::uchar4x3", "Math::uchar4x4",
    "short", "Math::short2", "Math::short3", "Math::short4", "Math::short2x2", "Math::short2x3", "Math::short2x4", "Math::short3x2", "Math::short3x3", "Math::short3x4", "Math::short4x2", "Math::short4x3", "Math::short4x4",
    "ushort", "Math::ushort2", "Math::ushort3", "Math::ushort4", "Math::ushort2x2", "Math::ushort2x3", "Math::ushort2x4", "Math::ushort3x2", "Math::ushort3x3", "Math::ushort3x4", "Math::ushort4x2", "Math::ushort4x3", "Math::ushort4x4",
    "int", "Math::int2", "Math::int3", "Math::int4", "Math::int2x2", "Math::int2x3", "Math::int2x4", "Math::int3x2", "Math::int3x3", "Math::int3x4", "Math::int4x2", "Math::int4x3", "Math::int4x4",
    "uint", "Math::uint2", "Math::uint3", "Math::uint4", "Math::uint2x2", "Math::uint2x3", "Math::uint2x4", "Math::uint3x2", "Math::uint3x3", "Math::uint3x4", "Math::uint4x2", "Math::uint4x3", "Math::uint4x4",
    "long", "Math::long2", "Math::long3", "Math::long4", "Math::long2x2", "Math::long2x3", "Math::long2x4", "Math::long3x2", "Math::long3x3", "Math::long3x4", "Math::long4x2", "Math::long4x3", "Math::long4x4",
    "ulong", "Math::ulong2", "Math::ulong3", "Math::ulong4", "Math::ulong2x2", "Math::ulong2x3", "Math::ulong2x4", "Math::ulong3x2", "Math::ulong3x3", "Math::ulong3x4", "Math::ulong4x2", "Math::ulong4x3", "Math::ulong4x4"
};


std::string escapeString(const std::string& input) {
    std::string output;
    output.reserve(input.size() * 2); // overkill to avoid frequer allocations

    for (char c : input) {
        switch (c) {
        case '"':  output += "\\\""; break;
        case '\\': output += "\\\\"; break;
        case '\b': output += "\\b";  break;
        case '\f': output += "\\f";  break;
        case '\n': output += "\\n";  break;
        case '\r': output += "\\r";  break;
        case '\t': output += "\\t";  break;
        default:
            if (c < 0x20) {
                // unicode bullshit idk
                output += "\\u";
                output += "0123456789abcdef"[(c >> 12) & 0xF];
                output += "0123456789abcdef"[(c >> 8) & 0xF];
                output += "0123456789abcdef"[(c >> 4) & 0xF];
                output += "0123456789abcdef"[c & 0xF];
            }
            else {
                output += c;
            }
            break;
        }
    }

    return "\"" + output + "\"";
}


size_t getIndentCount(const std::string& str) {
    size_t count = 0;
    for (char ch : str) {
        if (std::isspace(static_cast<unsigned char>(ch))) {
            ++count;
        }
        else {
            break;
        }
    }
    return count;
}


void replaceAll(std::string& str, const std::string& from, const std::string& to, size_t pos = 0) {
    if (from.empty()) return;

    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length(); // Move past the replacement
    }
}


bool verifyPath(const Amino::String& filename) {
    if (filename.empty()) {
        return false;
    }

    std::filesystem::path filePath(filename.c_str());
    if (!std::filesystem::exists(filePath.parent_path())) {
        try {
            if (!std::filesystem::create_directories(filePath.parent_path())) {
                return false;
            }
        }
        catch (const std::filesystem::filesystem_error&) {
            return false;
        }
    }
    return true;
}


void Hyuu::File::read_text_file(const Amino::String& filename, Amino::String& text)
{
	std::ifstream file(filename.c_str());

	if (!file.is_open()) {
		text = "";
		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	text = buffer.str();

	file.close();
}


void Hyuu::File::write_text_file(const Amino::String& text, const Amino::String& filename, bool& success)
{
	if (!verifyPath(filename)) {
		success = false;
		return;
	}

	std::ofstream file(filename.c_str());
	if (!file.is_open()) {
		success = false;
		return;
	}

	file << text.c_str();
	file.close();
	success = true;
}


void Hyuu::File::write_json(Bifrost::Object& object, const Amino::String& filename, bool& success)
{
	if (!verifyPath(filename)) {
		success = false;
		return;
	}

    std::ofstream file(filename.c_str());
    if (!file.is_open()) {
        success = false;
        return;
    }

	std::stringstream stream;
	Bifrost::Geometry::debugDump(stream, object, std::numeric_limits<size_t>::max());

    std::string line;
    std::smatch match;
    size_t pos;

    success = true;
    bool bPendingComma = false;
    int LastIndentCount = 0;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        {
            int IndentCount = getIndentCount(line);
            if (bPendingComma) {
                if (IndentCount == LastIndentCount)
                    file << ",";
                file << "\n";
            }
            bPendingComma = false;
            LastIndentCount = IndentCount;
        }

        // end of object
        if (line[line.length() - 1] == '}') {
            file << line;
            bPendingComma = true;
            continue;
        }

        // end of object array
        if (line[line.length() - 1] == ']') {
            file << line;
            bPendingComma = true;
            continue;
        }

        // erase type
        pos = line.find_last_of('(');
        if (pos == std::string::npos) {
            // if no type information there's nothing more to be done
            if (line.find(": ") == std::string::npos) {
                file << line << "\n";
            }
            else {
                file << std::string(LastIndentCount, ' ') << '"'
                    << line.substr(LastIndentCount, line.length() - LastIndentCount - 2) << "\": \n";
            }
            continue;
        }

        // strip and parse type
        std::string type_str = line.substr(pos + 1, line.length() - pos - 2);
        line.erase(pos - 1);
        bool bArray = type_str.find("array<") == 0;
        if (bArray) {
            type_str = type_str.substr(6, type_str.length() - 7);
        }

        // remove element count
        if (bArray) {
            pos = line.find_last_of(']');
            if (pos == std::string::npos) {
                file << "ERROR: Failed to find array closing bracket -> " << line << "\n";
                success = false;
                bPendingComma = false;
                continue;
            }
            line.erase(pos + 1);
        }

        // split data and key
        pos = line.find(": ");
        if (pos == std::string::npos) {
            file << "ERROR: Failed to find key split pos -> " << line << "\n";
            success = false;
            bPendingComma = false;
            continue;
        }
        std::string key = line.substr(LastIndentCount, pos - LastIndentCount);
        std::string data = line.substr(pos + 2);

        // Convert bool
        if (type_str == "bool") {
            replaceAll(data, "1", "true");
            replaceAll(data, "0", "false");
        }
        // Convert numeric
        else if (NUMERIC_TYPES.find(type_str) != NUMERIC_TYPES.end()) {
            std::replace(data.begin(), data.end(), '{', '[');
            std::replace(data.begin(), data.end(), '}', ']');
            if (data.find('f') != std::string::npos) {
                data.erase(std::remove(data.begin(), data.end(), 'f'), data.end());
            }

        }
        // treat everything else as string
        else {
            // I can't parse string arrays, without quotes there's no way
            // to distinguish if ', ' is part of a str or a new item
            data = escapeString(data);
        }


        file << std::string(LastIndentCount, ' ') << '"' << key << "\": " << data;
        bPendingComma = true;
    }

    file.close();
    return;
}