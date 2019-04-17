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
	g_ICvar->ConsolePrintf("%u\n", g_ISteamUser->GetSteamID().GetAccountID());
});

static InitRoutine init([]() {
	static BytePatch p[2] = {
		{gSignatures.GetClientSignature,
			"55 89 E5 53 83 EC 14 8B 45 08 8B 40 30",
			0x00, {0x31, 0xC0, 0x40, 0xC3}},
		{gSignatures.GetClientSignature,
			"55 89 E5 57 56 53 83 EC ? 8B 45 0C 8B 5D 08 8B 55 10 89 45 ? 8B 43",
			0x00, {0x31, 0xC0, 0x40, 0xC3}}
	};
	if (*pb) for (int i = 0; i < 2; ++i)
		p[i].Patch();
	pb.installChangeCallback([](settings::VariableBase<bool> &var, bool new_val) {
		for (int i = 0; i < 2; ++i)
			if (new_val)	p[i].Patch();
			else			p[i].Shutdown();
	});
});
/* <<Aye aye captain!>> */
