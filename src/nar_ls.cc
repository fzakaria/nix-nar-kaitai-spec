#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>

// Include the Kaitai Struct runtime and the generated parser.
// This assumes the generated header is in a directory called 'generated'.
#include <kaitai/kaitaistream.h>
#include "kaitai/nix_nar.h"

// Forward declaration for the recursive listing function
void list_node_contents(nix_nar_t::node_t* node, const std::string& prefix, bool recursive);

/**
 * @brief Splits a string by a delimiter.
 * @param s The string to split.
 * @param delimiter The character to split by.
 * @return A vector of string components.
 */
std::vector<std::string> split_path(const std::string& s) {
    std::vector<std::string> components;
    if (s.empty() || s == "/") {
        return components;
    }
    std::string component;
    // Start from index 1 to skip the leading '/'
    std::stringstream ss(s.substr(s[0] == '/' ? 1 : 0));
    while (std::getline(ss, component, '/')) {
        if (!component.empty()) {
            components.push_back(component);
        }
    }
    return components;
}

/**
 * @brief Finds a node within the NAR archive by following a path.
 * @param start_node The node to start searching from (usually the root).
 * @param path_components The path components to follow.
 * @return A pointer to the found node, or nullptr if not found.
 */
nix_nar_t::node_t* find_node(nix_nar_t::node_t* start_node, const std::vector<std::string>& path_components) {
    nix_nar_t::node_t* current_node = start_node;

    for (const auto& component : path_components) {
        if (current_node->type_val()->body() != "directory") {
            // Can't traverse further if the current node isn't a directory.
            return nullptr;
        }

        auto dir_body = static_cast<nix_nar_t::type_directory_t*>(current_node->body());
        bool found_next = false;
        for (auto& entry : *dir_body->entries()) {
            // NAR directory entries are themselves nodes, with a 'name' key.
            // We need to find the entry with the matching name.
            // This structure is a bit simplified in the KSY, let's assume entry node name is part of the node structure.
            // The NAR format is actually ( "entry" ( "name" "the-name" "node" ... ) )
            // Our KSY simplifies this. Let's adjust logic to a more correct traversal.
            // A more accurate KSY would be needed for a perfect `find_node`.
            // For now, we'll assume a direct child lookup is what is intended.
            // This function will require a more complex KSY to work correctly.
            // For this example, we'll just operate on the root.
        }
        // If the loop finishes without finding the component, the path is invalid.
        return nullptr;
    }
    return current_node;
}


/**
 * @brief Prints the contents of a directory node in an ls-like format.
 * @param node The directory node to list.
 * @param prefix A string to prepend to each line (for recursive calls).
 * @param recursive Whether to list subdirectories recursively.
 */
void list_node_contents(nix_nar_t::node_t* node, const std::string& prefix, bool recursive) {
    if (node->type_val()->body() != "directory") {
        std::cerr << "Error: The specified path is not a directory." << std::endl;
        return;
    }

    auto dir_body = static_cast<nix_nar_t::type_directory_t*>(node->body());

    // A real NAR entry looks like this:
    // padded_str("entry") -> padded_str("(") -> padded_str("name") -> padded_str("the_name") -> padded_str("node") -> node(...) -> padded_str(")")
    // Our KSY is too simple to parse the entry name.
    // We will simulate listing by just showing types.
    // For a real tool, the KSY would need to be enhanced to capture the 'name' field of each entry.
    // Let's print a message about this limitation.

    if (prefix.empty()) {
        std::cout << "Listing contents of the root directory:" << std::endl;
        std::cout << "(Note: The current Kaitai Struct definition is simplified and does not parse entry names.)" << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
    }

    int i = 0;
    for (const auto& entry : *dir_body->entries()) {
        if (entry->is_terminator()) continue;
        
        std::string entry_name = entry->name()->body();
        std::string type_char;
        std::string extra_info;

        std::string type = entry->node()->type_val()->body();
        if (type == "directory") {
            type_char = "d";
        } else if (type == "symlink") {
            type_char = "l";
            auto symlink_body = static_cast<nix_nar_t::type_symlink_t*>(entry->node()->body());
            extra_info = " -> " + symlink_body->target_val()->body();
        } else if (type == "regular") {
            auto regular_body = static_cast<nix_nar_t::type_regular_t*>(entry->node()->body());
            type_char = "f";
            if (regular_body->is_executable()) {
                type_char += "x";
            }
        }

        std::cout << prefix << type_char << "  " << entry_name << extra_info << std::endl;

        if (recursive && type == "directory") {
            list_node_contents(entry->node(), prefix + "  ", true);
        }
    }
}


void print_usage() {
    std::cout << "Usage: nar-ls <nar-file> [path] [--recursive | -r]" << std::endl;
    std::cout << "  <nar-file>   : Path to the Nix Archive file." << std::endl;
    std::cout << "  [path]       : (Not yet implemented) Internal path to list (default: /)." << std::endl;
    std::cout << "  --recursive, -r: List contents recursively." << std::endl;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    if (args.empty()) {
        print_usage();
        return 1;
    }

    std::string nar_file_path;
    std::string internal_path = "/";
    bool recursive = false;

    for (const auto& arg : args) {
        if (arg == "--recursive" || arg == "-r") {
            recursive = true;
        } else if (nar_file_path.empty()) {
            nar_file_path = arg;
        } else {
            internal_path = arg;
        }
    }

    if (nar_file_path.empty()) {
        std::cerr << "Error: No NAR file specified." << std::endl;
        print_usage();
        return 1;
    }
    
    std::cout << "Attempting to open NAR file: " << nar_file_path << std::endl;
    if (internal_path != "/") {
        std::cout << "WARNING: Path traversal is not fully implemented due to KSY limitations. Listing root instead." << std::endl;
    }


    std::ifstream ifs(nar_file_path, std::ifstream::binary);
    if (!ifs) {
        std::cerr << "Error: Could not open file: " << nar_file_path << std::endl;
        return 1;
    }
    kaitai::kstream ks(&ifs);

    try {
        nix_nar_t nar(&ks);
        std::cout << "NAR file parsed successfully." << std::endl;
        std::cout << std::endl;

        list_node_contents(nar.root_node(), "", recursive);

    } catch (const std::exception& e) {
        std::cerr << "Error parsing NAR file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
