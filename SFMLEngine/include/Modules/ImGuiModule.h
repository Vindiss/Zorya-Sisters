#pragma once

#include "Module.h"
#include "TimeModule.h"
#include "WindowModule.h"

class ImGuiModule final : public Module
{
	void Start() override;
	void Update() override;
	void Finalize() override;
	void Render() override;

	WindowModule* windowModule = nullptr;
	TimeModule* timeModule = nullptr;

protected:
	~ImGuiModule() = default;
};
