#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdint.h>
#include <seccomp.h>


int check(char* code) {

	for (int i = 0; i < 0x1000; i += 1) {

		// block our syscall bytes the LAZY way:)
		if (code[i] == 0x0f || code[i] == 0x05 || code[i] == 0xcd || code[i] == 0x80)
			return 1;
	}

	// install seccomp filters as extra security!!
	// it shouldn't matter though, since syscall is blocked anyways
	scmp_filter_ctx ctx;
	ctx = seccomp_init(SCMP_ACT_KILL);
	if (!ctx)
		return 1;
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 0) < 0)
		return 1;
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 0) < 0)
		return 1;
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 0) < 0)
		return 1;
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0) < 0)
		return 1;
	if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0) < 0)
		return 1;

	seccomp_load(ctx);
}

int main() {

	setbuf(stdin, 0);
	setbuf(stdout, 0);

	void (*fptr)(void);
	char* code = mmap(0, 0x1000, 7, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
	fptr = (void(*)(void))code;

	printf("Blob Runner> ");
	fgets(code, 0x1000, stdin);

	if (!check(code)) {
		fptr();
	} else {
		printf("Bad Blob!\n");
	}

}
