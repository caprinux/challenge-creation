# Challenge Name

re:life

# Description

Doesn't it suck that you only live once? In the all new isekai RE:LIFE simulator, you get to live and relive! Isn't that awesome?

# Summary

integer overflow gives us a buffer overflow in BSS. execv functionality allows us to brute force ASLR to put BSS adjacent to heap.

bss to heap overflow allows us to overwrite tcache\_perthread\_struct and gain arbitrary allocation

https://github.com/n132/BeapOverflow

# Author

Jin Kai

# Hints

My script can take as long as 10 minutes to run on remote on a bad day :(

# Flag

`grey{not_every_life_is_equal_you_are_one_in_eight_thousand_one_hundred_and_ninety_two!}`
