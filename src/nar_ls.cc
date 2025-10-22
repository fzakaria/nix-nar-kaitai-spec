#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Include the Kaitai Struct runtime and the generated parser.
// This assumes the generated header is in a directory called 'generated'.
#include <absl/debugging/failure_signal_handler.h>
#include <absl/debugging/stacktrace.h>
#include <absl/debugging/symbolize.h>
#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <absl/flags/usage.h>
#include <absl/log/globals.h>
#include <absl/log/initialize.h>
#include <absl/log/log.h>
#include <absl/status/statusor.h>
#include <absl/strings/str_cat.h>
#include <json.h>
#include <kaitai/kaitaistream.h>

#include "kaitai/nix_nar.h"

/**
 * @brief Prints the contents of a directory node in an ls-like format.
 * @param node The directory node to list.
 */
absl::StatusOr<jt::Json> list_node_contents(nix_nar_t::node_t* node) {
  if (node == nullptr) {
    return absl::InvalidArgumentError("Error: Node is null.");
  }

  if (node->type_val()->body() != "directory") {
    return absl::InvalidArgumentError(
        "Error: The specified path is not a directory.");
  }

  jt::Json result = jt::Json();
  result["type"] = "directory";
  result["entries"] = jt::Json();

  auto dir_body = static_cast<nix_nar_t::type_directory_t*>(node->body());

  for (const auto& entry : *dir_body->entries()) {
    if (entry->is_terminator()) {
      break;
    }

    std::string entry_name = entry->name()->body();
    std::string type = entry->node()->type_val()->body();
    result["entries"][entry_name] = jt::Json();
    result["entries"][entry_name].setObject();

    if (type == "directory") {
      const auto status = list_node_contents(entry->node());
      if (!status.ok()) {
        return status;
      }
      result["entries"][entry_name] = *status;
    }
  }

  return result;
}

int main(int argc, char* argv[]) {
  absl::InitializeLog();
  absl::SetStderrThreshold(absl::LogSeverityAtLeast::kInfo);
  absl::InitializeSymbolizer(argv[0]);
  absl::InstallFailureSignalHandler({});
  absl::SetProgramUsageMessage(R"(List contents of a Nix Archive (NAR) file.

Usage: nar-ls <nar-file>
  <nar-file>   : Path to the Nix Archive file.
)");

  std::vector<char*> positional_args = absl::ParseCommandLine(argc, argv);
  // The first positional argument is the program name.
  positional_args.erase(positional_args.begin());

  if (positional_args.size() < 1) {
    LOG(ERROR) << "Error: You must provide a NAR file.";
    return 1;
  }

  std::string nar_file_path = positional_args[0];
  std::ifstream ifs(nar_file_path, std::ifstream::binary);
  if (!ifs) {
    LOG(ERROR) << "Error: Could not open file: " << nar_file_path;
    return 1;
  }
  kaitai::kstream ks(&ifs);

  nix_nar_t nar(&ks);
  absl::StatusOr<jt::Json> status = list_node_contents(nar.root_node());
  if (!status.ok()) {
    LOG(ERROR) << status.status().message();
    return 1;
  }

  std::cout << status->toString() << std::endl;
  return 0;
}
