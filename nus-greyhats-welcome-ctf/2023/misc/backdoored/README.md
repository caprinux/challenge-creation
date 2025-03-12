# Challenge Details

MISSION BRIEF FOR AGENT 1337:

We have been tracking a malicious hacker, TACYERG, for a long time now and we finally have found a lead that might reveal their identity.

We have identified a compromised computer where TACYERG has inflitrated via a backdoor, and we have managed to capture a network traffic of his infiltration.

Can you follow the traces and find out what TACYERG is up to, and possibly find clues that might help us catch him?

# Solution

Provided PCAP file only shows how the attacker used the backdoor to get into the compromised computer.

We can replay the packets, in order to gain access to the server, and look through `~/.bash_history` to find traces of what the attacker has done.

Inside `~/.bash_history`, shows that attacker ran the `vim /tmp/.script.sh` command. However, when the participant tries to read the file, it is empty.

However, when the participant tries to read the file, it is empty.

Apart from `bash_history`, there is also a `~/.vimrc` file that shows that the undofile configuration is enabled amongst other config.

By looking for the undofile, participants can then find the flag that has been deleted.

# Flag

`greyhats{n1c3_w3_g0tt3m_ahahahahahhahaahahha}`
