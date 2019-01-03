/* This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include <settings/Bool.hpp>
#include "common.hpp"

static settings::Bool pb("party-bypass", "true");

static CatCommand identify("print_steamid", "Prints your SteamID",
[]() {
	auto s = std::to_string(g_ISteamUser->GetSteamID().GetAccountID());
	g_ICvar->ConsolePrintf("%s\n", s.c_str());
});

static InitRoutine init([]() {
	static BytePatch p[] = {
		{gSignatures.GetClientSignature,
			"74 ? E8 ? ? ? ? 89 F1",
			0x00, {0x90, 0x90}},
		{gSignatures.GetClientSignature,
			"0F 84 ? ? ? ? 8B 45 ? 8B 70 ? 8B 78 ? 8D",
			0x00, {0xBB, 0x01, 0x00, 0x00, 0x00, 0x90}},
		{gSignatures.GetClientSignature,
			"0F 84 ? ? ? ? 8B 73 ? 8D 45",
			0x00, {0x90, 0xE9}},
		{gSignatures.GetClientSignature,
			"0F 84 ? ? ? ? 8B 7B ? 8D 45",
			0x00, {0xC6, 0x85, 0x8F, 0xFE, 0xFF, 0xFF, 0x01}},
		{gSignatures.GetClientSignature,
			"8B 85 ? ? ? ? 89 44 24 ? 8B 85 ? ? ? ? 89 04 24 E8 ? ? ? ? E9 ? ? ? ? 0F",
			0x00, {0x8B, 0x85, 0xF4, 0xFD, 0xFF, 0xFF}},
		{gSignatures.GetClientSignature,
			"0F 84 ? ? ? ? 80 BD ? ? ? ? ? 0F 84 ? ? ? ? 8D 85 ? ? ? ? 89 44",
			0x00, {0x90, 0x90, 0x90, 0x90, 0x90, 0x90}},
		{gSignatures.GetClientSignature,
			"0F 84 ? ? ? ? 80 BD ? ? ? ? ? 0F 84 ? ? ? ? 8D 85 ? ? ? ? 89 44",
			0x0D, {0x0F, 0x8E}}
	};
	if (*pb) for (int i = 0; i < 7; ++i)
		p[i].Patch();
	pb.installChangeCallback([](settings::VariableBase<bool> &var, bool new_val) {
		for (int i = 0; i < 7; ++i)
			if (new_val) 	p[i].Patch();
			else 			p[i].Shutdown();
	});
});
/* <<Aye aye captain!>> */
/* todo?
 * 68 74 74 70 ?  ?  ?  ?  79 6F 75 74 75 ?  62 65 ?  4B 6A 59 4D 49 4A 51 7A 4B 35 6F
 */
