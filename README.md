Party bypass allows you to join MM party without friending with another player.
You only need to know other player's ID. Both players have to enable party bypass for this to work.
This is module for [cathook](https://github.com/nullworks/cathook), so cathook is required for this to work

## How to install ?

1. Copy PartyBypass.cpp to directory called "modules"
2. (Re-)run CMake to include new file
3. (Re-)compile cathook
4. Party bypass enabled by default, enjoy! Use `tf_party_request_join_user <steam_id>` to join party you want. Use `print_steamid` to get your SteamID.

Note: menu option is not added. To toggle bypass use `cat set party-bypass <true|false>`

Happy New year!
