#pragma once

/*--- numeric reply --*/
# define RPL_MODEIS                 "324" // MODE <channel> <mode> <param>
# define RPL_TOPIC                  "332" // TOPIC sendign the topic of the channel
# define RPL_NOTOPIC                "331" // TOPIC no topic has been set
# define RPL_INVITING               "341" // INVITE sent to the one doing the inviting
# define RPL_YOUREOPER              "381" // OPER Success Reply
# define RPL_CHANNELMODES           "324" // MODE
# define ERR_NORULES	            "434"
# define ERR_NOPRIVILEGES           "481" // MODE RETURNED BY MODE +o/-o to set a Chanop
# define ERR_NICKNAMEINUSE          "433" // NICK when a nickname is already in use
# define ERR_NICKCOLLISION          "436" // NICK nickname Collision
# define ERR_NEEDMOREPARAMS         "461" // When a command has more or less params
# define ERR_INVITEONLYCHAN         "473" // JOIN When channel has invite Mode on
# define ERR_NONICKNAMEGIVEN        "431" // NICK when Nickname isnt provided
# define ERR_USERNOTINCHANNEL       "441" // KICK when trying to kick a user who isnt in channel 
# define ERR_CHANOPRIVSNEEDED       "482" // Returned by commands that need special ChanOp prevs :: KICK,INVITE,MODE,TOPIC
# define EERR_ERRONEUSNICKNAME	    "432" // NICK : Invalid NickName
# define ERR_ALREADYREGISTERED      "462" // 
# define ERR_TOOMANYCHANNELS        "405" // JOIN & PART when a user wants to access multiple channels
# define ERR_BADCHANNELKEY          "475" // Join when the channel has key and user gave the wrong key
# define ERR_NOSUCHCHANNEL          "403" // When the channel doesnt exist // MODE / JOIN / INVITE / TOPIC 
# define ERR_CHANNELISFULL          "471" // Join when User wants to join a Mode +l channel and is full
# define ERR_NOPRIVS                "491"
# define RPL_WELCOME                "001" // Authentication
# define ERR_PASSWDMISMATCH         "464" // Join and Oper
# define ERR_KEYSET                 "467" // Mode +k : case of when the key is already set and the user wants to set it again
# define ERR_KICKEDFROMCHAN         "470" // Join when the user is kicked & Kick when the user has been kicked
# define ERR_NOCHANMODES            "477" // Mode for lobby
# define ERR_CANNOTCHANGECHANMODE   "974" // When Mode fails
# define ERR_CANNOTCHANGEUMODE      "973" // When at authentication Mode +i is sent by irssi
# define ERR_UNKNOWNMODE            "472" // When Mode has something other than itlok
# define ERR_USERONCHANNEL          "443" // When User is already is already on channel and is invited to
# define ERR_ERRONEUSNICKNAME       "432"
# define RPL_ENDOFBANLIST           "368"
# define RPL_CHANNELMODEIS          "324"
# define RPL_NAMREPLY               "353"
# define RPL_ENDOFNAMES             "366"
# define ERR_NOSUCHNICK				"401"
# define RPL_UMODEIS                "221"