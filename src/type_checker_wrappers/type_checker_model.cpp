#include "type_checker_wrappers/type_checker_model.hpp"
#include "compiler_state.hpp"
#include "constraint_list_model.hpp"
#include "qobject.h"
#include "subst_list_model.hpp"
#include "type_checker_state_model.hpp"

TypeCheckerStateModel* TypeCheckerModel::getState(int index) {
    return states[static_cast<size_t>(index)].get();
}
void TypeCheckerModel::setCompiler(CompilerState* state) {
    compiler = state;
    QObject::connect(compiler, &CompilerState::statusChanged, this, &TypeCheckerModel::compilerStatusChanged);
}
void TypeCheckerModel::compilerStatusChanged() {
    if (compiler->status != CompilerState::Ready) {
        dirty = true;
        return;
    }
    if (dirty) prepareStates();
}

void TypeCheckerModel::prepareStates() {
    if(!dirty) return;
    states.clear(); raw_states_buffer.clear();
    if (compiler->status != CompilerState::Ready) return;

    auto& info = compiler->output.compilation_info.function_info;
    if(!info.contains(functionName.toStdString())) return;
    auto& this_info = info.at(functionName.toStdString());
    auto& initial = this_info.initial_state;
    states.emplace_back(new TypeCheckerStateModel(this, &initial));
    auto* current_state = &initial;
    for(auto& step : this_info.steps) {
        current_state = &raw_states_buffer.emplace_back(current_state->applied(std::move(step)));
    }
    std::ranges::transform(raw_states_buffer, std::back_inserter(states), [this](auto& in) {
        return std::make_unique<TypeCheckerStateModel>(this, &in);
    });
    dirty = false;
}

ConstraintListModel* TypeCheckerStateModel::getActiveConstraints() {
    return new ConstraintListModel(nullptr, inner->active_constraints);
}

ConstraintListModel* TypeCheckerStateModel::getGeneratedConstraints() {
    return new ConstraintListModel(nullptr, inner->generated_constraints);
}

SubstListModel* TypeCheckerStateModel::getSubstitutions() {
    return new SubstListModel(nullptr, inner->subsitutions);
}
