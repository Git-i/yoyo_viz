#include "include/compiler_state.hpp"
#include <csignal>
#include "tree_cloner.h"
#include <memory>
#include <ranges>
void CompilerState::compile() {
    if (status == Compiling) return;
    if (compilation_thread.joinable()) {
        compilation_thread.join();
    }
    setStatus(Compiling);
    auto source = document->textDocument()->toPlainText().toStdString();
    std::thread temp([this, source]() {
        std::string module_name = "source";
        engine.removeModule(module_name);
        auto mod = engine.addModule(module_name, source);
        if (mod == nullptr) {
            // TODO: handle setting error details
            setStatus(Error);
            return;
        }
        auto source = engine.get_module_parse_output(module_name);
        syntax_tree.clear();
        std::ranges::move((*source) | std::views::transform([](const std::unique_ptr<Yoyo::Statement>& in) {
            return Yoyo::StatementTreeCloner::copy_stat(in.get(), nullptr);
        }), std::back_inserter(syntax_tree));
        auto result = engine.compile();
        if (!result.is_successful()) {
            setStatus(Error); return;
        }
        this->output = std::move(result.compiled_modules[mod]);
        setStatus(Ready);
    });
    compilation_thread.swap(temp);
}
void CompilerState::setStatus(Status s) {
    status = s;
    emit statusChanged(); 
}
CompilerState::~CompilerState() {

}
