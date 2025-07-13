#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <complex.h>
#include <locale.h>
#include <sched.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <termios.h>
#include <utmp.h>
#include <utmpx.h>
#include <grp.h>
#include <pwd.h>
#include <glob.h>
#include <regex.h>
#include <iconv.h>
#include <libintl.h>
#include <crypt.h>
#include <fenv.h>
#include <monetary.h>
#include <search.h>
#include <aio.h>
#include <wordexp.h>
#include <obstack.h>
#include <argp.h>
#include <argz.h>
#include <envz.h>
#include <syslog.h>
#include <sys/utsname.h>
#include <sys/syscall.h>
#include <mntent.h>
#include <sys/timex.h>
#include <wctype.h>
#include <wchar.h>
#include <ucontext.h>
#include <setjmp.h>
#include <ftw.h>
#include <nl_types.h>
#include <malloc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <execinfo.h>

// This function will never be called - it's just to populate the GOT
void __attribute__((section(".fun"))) populate_got_table(void) {
    // Declare dummy variables for function calls
    time_t t = 0;
    int fd = -1, result = 0;
    size_t sz = 0;
    char *str = NULL, buf[1024];
    wchar_t *wstr = NULL, wbuf[1024];
    FILE *fp = NULL;
    void *ptr = NULL;
    struct stat st;
    struct sched_param sp;
    sigset_t sigset;
    DIR *dir = NULL;
    struct dirent *de = NULL;
    pid_t pid = 0;
    uid_t uid = 0;
    gid_t gid = 0;
    mode_t mode = 0;
    off_t offset = 0;
    struct sockaddr addr;
    socklen_t addrlen = 0;
    struct hostent *he = NULL;
    struct passwd *pw = NULL;
    struct group *gr = NULL;
    regex_t regex;
    glob_t globbuf;
    iconv_t cd;
    struct tm tm_buf;
    struct timeval tv;
    struct termios termios_buf;
    struct utmp utmp_buf;
    struct utmpx utmpx_buf;
    pthread_key_t key;
    sem_t sem;
    struct rlimit rlim;
    fenv_t fenv;
    mode_t femode;
    fexcept_t fexcept;
    struct aiocb aiocb_buf;
    wordexp_t we;
    struct obstack obs;
    struct argp argp_buf;
    struct argp_state argp_state;
    va_list ap;
    wctype_t wct;
    wctrans_t wctr;
    struct lconv *lc;
    nl_catd catd;
    struct mallinfo mi;
    jmp_buf jbuf;
    sigjmp_buf sjbuf;
    ucontext_t uctx;
    struct ntptimeval ntv;
    struct timex timex_buf;
    complex double cd_val = 0.0;
    complex float cf_val = 0.0f;
    complex long double cld_val = 0.0L;
    struct utsname uts;
    struct mntent mnt;
    struct if_nameindex *ifni;
    struct crypt_data crypt_data_buf;
    int64_t cmp_fn = NULL;
    __ftw_func_t ftw_fn = NULL;
    div_t div_result;
    ldiv_t ldiv_result;
    lldiv_t lldiv_result;
    int64_t imaxdiv_result;
    struct random_data random_data_buf;
    struct drand48_data drand48_data_buf;
    mbstate_t mbstate;
    ENTRY entry;
    uint16_t u16 = 0;
    uint32_t u32 = 0;
    speed_t speed = 0;
    tcflag_t tcflag = 0;
    double d = 0.0;
    float f = 0.0f;
    long double ld = 0.0L;
    long long int lli = 0;
    unsigned long long int ulli = 0;
    int64_t imax = 0;
    uint64_t uimax = 0;
    fpos_t fpos;

    // Call all the functions - these will never actually execute
        // Time functions
        time(&t);
        
        // File operations
        fcntl(fd, F_GETFL);
        open("dummy", O_RDONLY);
        open64("dummy", O_RDONLY);
        close(fd);
        read(fd, buf, sz);
        write(fd, buf, sz);
        pread(fd, buf, sz, offset);
        pread64(fd, buf, sz, offset);
        pwrite(fd, buf, sz, offset);
        pwrite64(fd, buf, sz, offset);
        lseek(fd, offset, SEEK_SET);
        lseek64(fd, offset, SEEK_SET);
        
        // String functions
        mbstowcs(wbuf, buf, sz);
        wcstombs(buf, wbuf, sz);
        strlen(buf);
        wcslen(wbuf);
        strnlen(buf, sz);
        wcsnlen(wbuf, sz);
        strcmp(buf, buf);
        wcscmp(wbuf, wbuf);
        strcasecmp(buf, buf);
        wcscasecmp(wbuf, wbuf);
        strncmp(buf, buf, sz);
        wcsncmp(wbuf, wbuf, sz);
        strncasecmp(buf, buf, sz);
        wcsncasecmp(wbuf, wbuf, sz);
        strverscmp(buf, buf);
        strcpy(buf, buf);
        wcscpy(wbuf, wbuf);
        strncpy(buf, buf, sz);
        wcsncpy(wbuf, wbuf, sz);
        strcat(buf, buf);
        wcscat(wbuf, wbuf);
        strncat(buf, buf, sz);
        wcsncat(wbuf, wbuf, sz);
        strchr(buf, 'a');
        wcschr(wbuf, L'a');
        strrchr(buf, 'a');
        wcsrchr(wbuf, L'a');
        strstr(buf, buf);
        wcsstr(wbuf, wbuf);
        strpbrk(buf, buf);
        wcspbrk(wbuf, wbuf);
        strspn(buf, buf);
        wcsspn(wbuf, wbuf);
        strcspn(buf, buf);
        wcscspn(wbuf, wbuf);
        strtok(buf, buf);
        wcstok(wbuf, wbuf, &wstr);
        strtok_r(buf, buf, &str);
        strsep(&str, buf);
        strdup(buf);
        wcsdup(wbuf);
        strndup(buf, sz);
        stpcpy(buf, buf);
        wcpcpy(wbuf, wbuf);
        stpncpy(buf, buf, sz);
        wcpncpy(wbuf, wbuf, sz);
        strchrnul(buf, 'a');
        wcschrnul(wbuf, L'a');
        strcasestr(buf, buf);
        wcswcs(wbuf, wbuf);
        index(buf, 'a');
        rindex(buf, 'a');
        
        // Memory functions
        memcpy(ptr, ptr, sz);
        wmemcpy(wbuf, wbuf, sz);
        mempcpy(ptr, ptr, sz);
        wmempcpy(wbuf, wbuf, sz);
        memmove(ptr, ptr, sz);
        wmemmove(wbuf, wbuf, sz);
        memccpy(ptr, ptr, 'a', sz);
        memset(ptr, 0, sz);
        wmemset(wbuf, L'a', sz);
        memcmp(ptr, ptr, sz);
        wmemcmp(wbuf, wbuf, sz);
        memchr(ptr, 'a', sz);
        wmemchr(wbuf, L'a', sz);
        rawmemchr(ptr, 'a');
        memrchr(ptr, 'a', sz);
        memmem(ptr, sz, ptr, sz);
        memfrob(ptr, sz);
        bcopy(ptr, ptr, sz);
        bcmp(ptr, ptr, sz);
        bzero(ptr, sz);
        explicit_bzero(ptr, sz);
        
        // Memory allocation
        malloc(sz);
        calloc(sz, sz);
        realloc(ptr, sz);
        free(ptr);
        aligned_alloc(sz, sz);
        memalign(sz, sz);
        posix_memalign(&ptr, sz, sz);
        valloc(sz);
        alloca(sz);
        
        // Math functions
        sin(d);
        sinf(f);
        sinl(ld);
        cos(d);
        cosf(f);
        cosl(ld);
        tan(d);
        tanf(f);
        tanl(ld);
        asin(d);
        asinf(f);
        asinl(ld);
        acos(d);
        acosf(f);
        acosl(ld);
        atan(d);
        atanf(f);
        atanl(ld);
        atan2(d, d);
        atan2f(f, f);
        atan2l(ld, ld);
        sinh(d);
        sinhf(f);
        sinhl(ld);
        cosh(d);
        coshf(f);
        coshl(ld);
        tanh(d);
        tanhf(f);
        tanhl(ld);
        asinh(d);
        asinhf(f);
        asinhl(ld);
        acosh(d);
        acoshf(f);
        acoshl(ld);
        atanh(d);
        atanhf(f);
        atanhl(ld);
        exp(d);
        expf(f);
        expl(ld);
        exp2(d);
        exp2f(f);
        exp2l(ld);
        exp10(d);
        exp10f(f);
        exp10l(ld);
        log(d);
        logf(f);
        logl(ld);
        log10(d);
        log10f(f);
        log10l(ld);
        log2(d);
        log2f(f);
        log2l(ld);
        logb(d);
        logbf(f);
        logbl(ld);
        ilogb(d);
        ilogbf(f);
        ilogbl(ld);
        llogb(d);
        llogbf(f);
        llogbl(ld);
        pow(d, d);
        powf(f, f);
        powl(ld, ld);
        sqrt(d);
        sqrtf(f);
        sqrtl(ld);
        cbrt(d);
        cbrtf(f);
        cbrtl(ld);
        hypot(d, d);
        hypotf(f, f);
        hypotl(ld, ld);
        expm1(d);
        expm1f(f);
        expm1l(ld);
        log1p(d);
        log1pf(f);
        log1pl(ld);
        floor(d);
        floorf(f);
        floorl(ld);
        ceil(d);
        ceilf(f);
        ceill(ld);
        trunc(d);
        truncf(f);
        truncl(ld);
        rint(d);
        rintf(f);
        rintl(ld);
        nearbyint(d);
        nearbyintf(f);
        nearbyintl(ld);
        round(d);
        roundf(f);
        roundl(ld);
        roundeven(d);
        roundevenf(f);
        roundevenl(ld);
        lrint(d);
        lrintf(f);
        lrintl(ld);
        llrint(d);
        llrintf(f);
        llrintl(ld);
        lround(d);
        lroundf(f);
        lroundl(ld);
        llround(d);
        llroundf(f);
        llroundl(ld);
        modf(d, &d);
        modff(f, &f);
        modfl(ld, &ld);
        frexp(d, &result);
        frexpf(f, &result);
        frexpl(ld, &result);
        ldexp(d, result);
        ldexpf(f, result);
        ldexpl(ld, result);
        scalb(d, d);
        scalbf(f, f);
        scalbl(ld, ld);
        scalbn(d, result);
        scalbnf(f, result);
        scalbnl(ld, result);
        scalbln(d, result);
        scalblnf(f, result);
        scalblnl(ld, result);
        significand(d);
        significandf(f);
        significandl(ld);
        fabs(d);
        fabsf(f);
        fabsl(ld);
        abs(result);
        labs(result);
        llabs(lli);
        imaxabs(imax);
        fmod(d, d);
        fmodf(f, f);
        fmodl(ld, ld);
        drem(d, d);
        dremf(f, f);
        dreml(ld, ld);
        remainder(d, d);
        remainderf(f, f);
        remainderl(ld, ld);
        copysign(d, d);
        copysignf(f, f);
        copysignl(ld, ld);
        nextafter(d, d);
        nextafterf(f, f);
        nextafterl(ld, ld);
        nexttoward(d, ld);
        nexttowardf(f, ld);
        nexttowardl(ld, ld);
        nextup(d);
        nextupf(f);
        nextupl(ld);
        nextdown(d);
        nextdownf(f);
        nextdownl(ld);
        nan("test");
        nanf("test");
        nanl("test");
        fmin(d, d);
        fminf(f, f);
        fminl(ld, ld);
        fmax(d, d);
        fmaxf(f, f);
        fmaxl(ld, ld);
        fminmag(d, d);
        fminmagf(f, f);
        fminmagl(ld, ld);
        fmaxmag(d, d);
        fmaxmagf(f, f);
        fmaxmagl(ld, ld);
        fdim(d, d);
        fdimf(f, f);
        fdiml(ld, ld);
        fma(d, d, d);
        fmaf(f, f, f);
        fmal(ld, ld, ld);
        isnan(d);
        isnanf(f);
        isnanl(ld);
        isinf(d);
        isinff(f);
        isinfl(ld);
        finite(d);
        finitef(f);
        finitel(ld);
        signbit(d);
        
        // Complex math functions
        csin(cd_val);
        csinf(cf_val);
        csinl(cld_val);
        ccos(cd_val);
        ccosf(cf_val);
        ccosl(cld_val);
        ctan(cd_val);
        ctanf(cf_val);
        ctanl(cld_val);
        casin(cd_val);
        casinf(cf_val);
        casinl(cld_val);
        cacos(cd_val);
        cacosf(cf_val);
        cacosl(cld_val);
        catan(cd_val);
        catanf(cf_val);
        catanl(cld_val);
        csinh(cd_val);
        csinhf(cf_val);
        csinhl(cld_val);
        ccosh(cd_val);
        ccoshf(cf_val);
        ccoshl(cld_val);
        ctanh(cd_val);
        ctanhf(cf_val);
        ctanhl(cld_val);
        casinh(cd_val);
        casinhf(cf_val);
        casinhl(cld_val);
        cacosh(cd_val);
        cacoshf(cf_val);
        cacoshl(cld_val);
        catanh(cd_val);
        catanhf(cf_val);
        catanhl(cld_val);
        cexp(cd_val);
        cexpf(cf_val);
        cexpl(cld_val);
        clog(cd_val);
        clogf(cf_val);
        clogl(cld_val);
        clog10(cd_val);
        clog10f(cf_val);
        clog10l(cld_val);
        csqrt(cd_val);
        csqrtf(cf_val);
        csqrtl(cld_val);
        cpow(cd_val, cd_val);
        cpowf(cf_val, cf_val);
        cpowl(cld_val, cld_val);
        creal(cd_val);
        crealf(cf_val);
        creall(cld_val);
        cimag(cd_val);
        cimagf(cf_val);
        cimagl(cld_val);
        conj(cd_val);
        conjf(cf_val);
        conjl(cld_val);
        carg(cd_val);
        cargf(cf_val);
        cargl(cld_val);
        cproj(cd_val);
        cprojf(cf_val);
        cprojl(cld_val);
        cabs(cd_val);
        cabsf(cf_val);
        cabsl(cld_val);
        
        // Special math functions
        erf(d);
        erff(f);
        erfl(ld);
        erfc(d);
        erfcf(f);
        erfcl(ld);
        lgamma(d);
        lgammaf(f);
        lgammal(ld);
        lgamma_r(d, &result);
        lgammaf_r(f, &result);
        lgammal_r(ld, &result);
        gamma(d);
        gammaf(f);
        gammal(ld);
        tgamma(d);
        tgammaf(f);
        tgammal(ld);
        j0(d);
        j0f(f);
        j0l(ld);
        j1(d);
        j1f(f);
        j1l(ld);
        jn(result, d);
        jnf(result, f);
        jnl(result, ld);
        y0(d);
        y0f(f);
        y0l(ld);
        y1(d);
        y1f(f);
        y1l(ld);
        yn(result, d);
        ynf(result, f);
        ynl(result, ld);
        
        // I/O functions
        fopen("dummy", "r");
        fopen64("dummy", "r");
        freopen("dummy", "r", fp);
        freopen64("dummy", "r", fp);
        fclose(fp);
        fcloseall();
        fdopen(fd, "r");
        fflush(fp);
        fflush_unlocked(fp);
        setbuf(fp, buf);
        setvbuf(fp, buf, _IOFBF, sz);
        setbuffer(fp, buf, sz);
        setlinebuf(fp);
        fread(ptr, sz, sz, fp);
        fread_unlocked(ptr, sz, sz, fp);
        fwrite(ptr, sz, sz, fp);
        fwrite_unlocked(ptr, sz, sz, fp);
        fgetc(fp);
        fgetwc(fp);
        fgetc_unlocked(fp);
        fgetwc_unlocked(fp);
        getc(fp);
        getwc(fp);
        getc_unlocked(fp);
        getwc_unlocked(fp);
        getchar();
        getwchar();
        getchar_unlocked();
        getwchar_unlocked();
        getw(fp);
        fputc('a', fp);
        fputwc(L'a', fp);
        fputc_unlocked('a', fp);
        fputwc_unlocked(L'a', fp);
        putc('a', fp);
        putwc(L'a', fp);
        putc_unlocked('a', fp);
        putwc_unlocked(L'a', fp);
        putchar('a');
        putwchar(L'a');
        putchar_unlocked('a');
        putwchar_unlocked(L'a');
        fputs(buf, fp);
        fputws(wbuf, fp);
        fputs_unlocked(buf, fp);
        fputws_unlocked(wbuf, fp);
        puts(buf);
        putw(result, fp);
        ungetc('a', fp);
        ungetwc(L'a', fp);
        fgets(buf, sz, fp);
        fgetws(wbuf, sz, fp);
        fgets_unlocked(buf, sz, fp);
        fgetws_unlocked(wbuf, sz, fp);
        getline(&str, &sz, fp);
        getdelim(&str, &sz, 'a', fp);
        feof(fp);
        feof_unlocked(fp);
        ferror(fp);
        ferror_unlocked(fp);
        clearerr(fp);
        clearerr_unlocked(fp);
        fileno(fp);
        fileno_unlocked(fp);
        fseek(fp, 0, SEEK_SET);
        fseeko(fp, 0, SEEK_SET);
        fseeko64(fp, 0, SEEK_SET);
        ftell(fp);
        ftello(fp);
        ftello64(fp);
        rewind(fp);
        fgetpos(fp, &fpos);
        fsetpos(fp, &fpos);
        
        // Printf family
        printf("test");
        wprintf(L"test");
        fprintf(fp, "test");
        fwprintf(fp, L"test");
        sprintf(buf, "test");
        swprintf(wbuf, sz, L"test");
        snprintf(buf, sz, "test");
        asprintf(&str, "test");
        vprintf("test", ap);
        vwprintf(L"test", ap);
        vfprintf(fp, "test", ap);
        vfwprintf(fp, L"test", ap);
        vsprintf(buf, "test", ap);
        vswprintf(wbuf, sz, L"test", ap);
        vsnprintf(buf, sz, "test", ap);
        vasprintf(&str, "test", ap);
        
        // Scanf family
        scanf("test");
        wscanf(L"test");
        fscanf(fp, "test");
        fwscanf(fp, L"test");
        sscanf(buf, "test");
        swscanf(wbuf, L"test");
        vscanf("test", ap);
        vwscanf(L"test", ap);
        vfscanf(fp, "test", ap);
        vfwscanf(fp, L"test", ap);
        vsscanf(buf, "test", ap);
        vswscanf(wbuf, L"test", ap);
        
        // Conversion functions
        strtol(buf, &str, 10);
        wcstol(wbuf, &wstr, 10);
        strtoul(buf, &str, 10);
        wcstoul(wbuf, &wstr, 10);
        strtoll(buf, &str, 10);
        wcstoll(wbuf, &wstr, 10);
        strtoq(buf, &str, 10);
        wcstoq(wbuf, &wstr, 10);
        strtoull(buf, &str, 10);
        wcstoull(wbuf, &wstr, 10);
        strtouq(buf, &str, 10);
        wcstouq(wbuf, &wstr, 10);
        strtoimax(buf, &str, 10);
        wcstoimax(wbuf, &wstr, 10);
        strtoumax(buf, &str, 10);
        wcstoumax(wbuf, &wstr, 10);
        strtod(buf, &str);
        wcstod(wbuf, &wstr);
        strtof(buf, &str);
        wcstof(wbuf, &wstr);
        strtold(buf, &str);
        wcstold(wbuf, &wstr);
        atoi(buf);
        atol(buf);
        atoll(buf);
        atof(buf);
        
        // Character classification
        isalnum('a');
        isalpha('a');
        iscntrl('a');
        isdigit('a');
        isgraph('a');
        islower('a');
        isprint('a');
        ispunct('a');
        isspace('a');
        isupper('a');
        isxdigit('a');
        isblank('a');
        isascii('a');
        tolower('a');
        toupper('a');
        toascii('a');
        _tolower('a');
        _toupper('a');
        
        // Wide character functions
        iswalnum(L'a');
        iswalpha(L'a');
        iswcntrl(L'a');
        iswdigit(L'a');
        iswgraph(L'a');
        iswlower(L'a');
        iswprint(L'a');
        iswpunct(L'a');
        iswspace(L'a');
        iswupper(L'a');
        iswxdigit(L'a');
        iswblank(L'a');
        towlower(L'a');
        towupper(L'a');
        wctype("alpha");
        iswctype(L'a', wct);
        wctrans("tolower");
        towctrans(L'a', wctr);
        
        // Multibyte functions
        mbsinit(&mbstate);
        mbtowc(wbuf, buf, sz);
        wctomb(buf, L'a');
        mblen(buf, sz);
        mbstowcs(wbuf, buf, sz);
        wcstombs(buf, wbuf, sz);
        btowc('a');
        wctob(L'a');
        mbrtowc(wbuf, buf, sz, &mbstate);
        mbrlen(buf, sz, &mbstate);
        wcrtomb(buf, L'a', &mbstate);
        mbsrtowcs(wbuf, (const char**)&str, sz, &mbstate);
        wcsrtombs(buf, (const wchar_t**)&wstr, sz, &mbstate);
        mbsnrtowcs(wbuf, (const char**)&str, sz, sz, &mbstate);
        wcsnrtombs(buf, (const wchar_t**)&wstr, sz, sz, &mbstate);
        
        // Process functions
        getpid();
        getppid();
        fork();
        vfork();
        system("true");
        wait(&result);
        waitpid(pid, &result, 0);
        wait3(&result, 0, NULL);
        wait4(pid, &result, 0, NULL);
        
        // Signal functions
        signal(SIGINT, SIG_DFL);
        sysv_signal(SIGINT, SIG_DFL);
        ssignal(SIGINT, SIG_DFL);
        raise(SIGINT);
        gsignal(SIGINT);
        kill(pid, SIGINT);
        killpg(pid, SIGINT);
        sigemptyset(&sigset);
        sigfillset(&sigset);
        sigaddset(&sigset, SIGINT);
        sigdelset(&sigset, SIGINT);
        sigismember(&sigset, SIGINT);
        sigprocmask(SIG_BLOCK, &sigset, NULL);
        sigpending(&sigset);
        sigsuspend(&sigset);
        sigaction(SIGINT, NULL, NULL);
        sigaltstack(NULL, NULL);
        sigstack(NULL, NULL);
        siginterrupt(SIGINT, 0);
        sigblock(0);
        sigsetmask(0);
        sigpause(0);
        
        // File system functions
        stat("dummy", &st);
        stat64("dummy", (struct stat64*)&st);
        fstat(fd, &st);
        fstat64(fd, (struct stat64*)&st);
        lstat("dummy", &st);
        lstat64("dummy", (struct stat64*)&st);
        access("dummy", R_OK);
        chmod("dummy", mode);
        fchmod(fd, mode);
        chown("dummy", uid, gid);
        fchown(fd, uid, gid);
        umask(mode);
        mkdir("dummy", mode);
        rmdir("dummy");
        unlink("dummy");
        remove("dummy");
        rename("dummy", "dummy2");
        link("dummy", "dummy2");
        symlink("dummy", "dummy2");
        readlink("dummy", buf, sz);
        realpath("dummy", buf);
        canonicalize_file_name("dummy");
        mknod("dummy", mode, 0);
        mkfifo("dummy", mode);
        
        // Directory functions
        opendir(".");
        fdopendir(fd);
        readdir(dir);
        readdir_r(dir, de, &de);
        readdir64(dir);
        readdir64_r(dir, (struct dirent64*)de, (struct dirent64**)&de);
        closedir(dir);
        rewinddir(dir);
        telldir(dir);
        seekdir(dir, 0);
        dirfd(dir);
        scandir(".", NULL, NULL, NULL);
        alphasort(NULL, NULL);
        versionsort(NULL, NULL);
        scandir64(".", NULL, NULL, NULL);
        alphasort64(NULL, NULL);
        versionsort64(NULL, NULL);
        
        // Memory mapping
        mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);
        mmap64(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);
        munmap(ptr, sz);
        msync(ptr, sz, MS_SYNC);
        mremap(ptr, sz, sz, 0);
        madvise(ptr, sz, MADV_NORMAL);
        mlock(ptr, sz);
        munlock(ptr, sz);
        mlockall(MCL_CURRENT);
        munlockall();
        shm_open("dummy", O_RDONLY, mode);
        shm_unlink("dummy");
        
        // Process memory
        brk(ptr);
        sbrk(0);
        
        // Scheduling
        sched_setscheduler(pid, SCHED_FIFO, &sp);
        sched_getscheduler(pid);
        sched_setparam(pid, &sp);
        sched_getparam(pid, &sp);
        sched_get_priority_min(SCHED_FIFO);
        sched_get_priority_max(SCHED_FIFO);
        sched_rr_get_interval(pid, NULL);
        sched_yield();
        sched_getaffinity(pid, sizeof(cpu_set_t), NULL);
        sched_setaffinity(pid, sizeof(cpu_set_t), NULL);
        
        // Process groups and sessions
        getpgrp();
        getpgid(pid);
        setpgid(pid, pid);
        setsid();
        getsid(pid);
        tcgetpgrp(fd);
        tcsetpgrp(fd, pid);
        tcgetsid(fd);
        
        // User/group ID functions
        getuid();
        geteuid();
        getgid();
        getegid();
        setuid(uid);
        seteuid(uid);
        setgid(gid);
        setegid(gid);
        setreuid(uid, uid);
        setregid(gid, gid);
        getgroups(0, NULL);
        setgroups(0, NULL);
        initgroups("user", gid);
        getgrouplist("user", gid, NULL, &result);
        
        // Password database
        getpwuid(uid);
        getpwuid_r(uid, pw, buf, sz, &pw);
        getpwnam("user");
        getpwnam_r("user", pw, buf, sz, &pw);
        fgetpwent(fp);
        fgetpwent_r(fp, pw, buf, sz, &pw);
        setpwent();
        getpwent();
        getpwent_r(pw, buf, sz, &pw);
        endpwent();
        putpwent(pw, fp);
        
        // Group database
        getgrgid(gid);
        getgrgid_r(gid, gr, buf, sz, &gr);
        getgrnam("group");
        getgrnam_r("group", gr, buf, sz, &gr);
        fgetgrent(fp);
        fgetgrent_r(fp, gr, buf, sz, &gr);
        setgrent();
        getgrent();
        getgrent_r(gr, buf, sz, &gr);
        endgrent();
        
        // Network functions
        socket(AF_INET, SOCK_STREAM, 0);
        bind(fd, &addr, addrlen);
        listen(fd, 5);
        accept(fd, &addr, &addrlen);
        connect(fd, &addr, addrlen);
        socketpair(AF_UNIX, SOCK_STREAM, 0, NULL);
        send(fd, buf, sz, 0);
        recv(fd, buf, sz, 0);
        sendto(fd, buf, sz, 0, &addr, addrlen);
        recvfrom(fd, buf, sz, 0, &addr, &addrlen);
        getsockopt(fd, SOL_SOCKET, SO_REUSEADDR, ptr, &addrlen);
        setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, ptr, addrlen);
        getsockname(fd, &addr, &addrlen);
        getpeername(fd, &addr, &addrlen);
        shutdown(fd, SHUT_RDWR);
        
        // Network byte order
        htons(u16);
        ntohs(u16);
        htonl(u32);
        ntohl(u32);
        
        // Network address functions
        inet_aton("127.0.0.1", NULL);
        inet_addr("127.0.0.1");
        inet_network("127.0.0.1");
        inet_ntoa(*(struct in_addr*)&u32);
        inet_makeaddr(u32, u32);
        inet_lnaof(*(struct in_addr*)&u32);
        inet_netof(*(struct in_addr*)&u32);
        inet_pton(AF_INET, "127.0.0.1", ptr);
        inet_ntop(AF_INET, ptr, buf, sz);
        
        // Network interface functions
        if_nametoindex("lo");
        if_indextoname(1, buf);
        if_nameindex();
        if_freenameindex(ifni);
        
        // Host database
        gethostbyname("localhost");
        gethostbyname2("localhost", AF_INET);
        gethostbyaddr(ptr, sz, AF_INET);
        gethostbyname_r("localhost", he, buf, sz, &he, &result);
        gethostbyname2_r("localhost", AF_INET, he, buf, sz, &he, &result);
        gethostbyaddr_r(ptr, sz, AF_INET, he, buf, sz, &he, &result);
        sethostent(1);
        gethostent();
        endhostent();
        
        // Network database
        getnetbyname("loopback");
        getnetbyaddr(u32, AF_INET);
        setnetent(1);
        getnetent();
        endnetent();
        
        // Protocol database
        getprotobyname("tcp");
        getprotobynumber(IPPROTO_TCP);
        setprotoent(1);
        getprotoent();
        endprotoent();
        
        // Service database
        getservbyname("http", "tcp");
        getservbyport(80, "tcp");
        setservent(1);
        getservent();
        endservent();
        
        // Netgroup functions
        setnetgrent("netgroup");
        getnetgrent(&str, &str, &str);
        getnetgrent_r(&str, &str, &str, buf, sz);
        endnetgrent();
        innetgr("netgroup", "host", "user", "domain");
        
        // Host identification
        gethostname(buf, sz);
        sethostname(buf, sz);
        setdomainname(buf, sz);
        gethostid();
        sethostid(0);
        uname(&uts);
        
        // Time functions
        time(&t);
        difftime(t, t);
        clock();
        times(NULL);
        gettimeofday(&tv, NULL);
        settimeofday(&tv, NULL);
        adjtime(&tv, &tv);
        adjtimex(&timex_buf);
        ntp_gettime(&ntv);
        ntp_adjtime(&timex_buf);
        
        // Time conversion
        localtime(&t);
        localtime_r(&t, &tm_buf);
        gmtime(&t);
        gmtime_r(&t, &tm_buf);
        mktime(&tm_buf);
        timelocal(&tm_buf);
        timegm(&tm_buf);
        asctime(&tm_buf);
        asctime_r(&tm_buf, buf);
        ctime(&t);
        ctime_r(&t, buf);
        strftime(buf, sz, "%Y", &tm_buf);
        wcsftime(wbuf, sz, L"%Y", &tm_buf);
        strptime(buf, "%Y", &tm_buf);
        getdate(buf);
        getdate_r(buf, &tm_buf);
        
        // Timezone
        tzset();
        
        // Sleep functions
        sleep(1);
        nanosleep(NULL, NULL);
        pause();
        
        // Random number functions
        rand();
        srand(1);
        rand_r((unsigned int*)&result);
        random();
        srandom(1);
        initstate(1, buf, sz);
        setstate(buf);
        random_r(&random_data_buf, &result);
        srandom_r(1, &random_data_buf);
        initstate_r(1, buf, sz, &random_data_buf);
        setstate_r(buf, &random_data_buf);
        drand48();
        erand48(NULL);
        lrand48();
        nrand48(NULL);
        mrand48();
        jrand48(NULL);
        srand48(1);
        seed48(NULL);
        lcong48(NULL);
        drand48_r(&drand48_data_buf, &d);
        erand48_r(NULL, &drand48_data_buf, &d);
        lrand48_r(&drand48_data_buf, (long int*)&result);
        nrand48_r(NULL, &drand48_data_buf, (long int*)&result);
        mrand48_r(&drand48_data_buf, (long int*)&result);
        jrand48_r(NULL, &drand48_data_buf, (long int*)&result);
        srand48_r(1, &drand48_data_buf);
        seed48_r(NULL, &drand48_data_buf);
        lcong48_r(NULL, &drand48_data_buf);
        
        // Environment
        getenv("PATH");
        secure_getenv("PATH");
        putenv("TEST=1");
        setenv("TEST", "1", 1);
        unsetenv("TEST");
        clearenv();
        
        // Locale
        setlocale(LC_ALL, "C");
        localeconv();
        
        // Internationalization
        textdomain("messages");
        bindtextdomain("messages", "/usr/share/locale");
        bind_textdomain_codeset("messages", "UTF-8");
        gettext("message");
        dgettext("messages", "message");
        dcgettext("messages", "message", LC_MESSAGES);
        ngettext("message", "messages", 1);
        dngettext("messages", "message", "messages", 1);
        dcngettext("messages", "message", "messages", 1, LC_MESSAGES);
        
        // Catalog functions
        catopen("catalog", 0);
        catgets(catd, 1, 1, "default");
        catclose(catd);
        
        // Terminal I/O
        isatty(fd);
        ttyname(fd);
        ttyname_r(fd, buf, sz);
        ctermid(buf);
        tcgetattr(fd, &termios_buf);
        tcsetattr(fd, TCSANOW, &termios_buf);
        tcsendbreak(fd, 0);
        tcdrain(fd);
        tcflush(fd, TCIFLUSH);
        tcflow(fd, TCOOFF);
        cfgetospeed(&termios_buf);
        cfgetispeed(&termios_buf);
        cfsetospeed(&termios_buf, speed);
        cfsetispeed(&termios_buf, speed);
        cfsetspeed(&termios_buf, speed);
        cfmakeraw(&termios_buf);
        
        // Pseudo-terminal functions
        getpt();
        grantpt(fd);
        unlockpt(fd);
        ptsname(fd);
        ptsname_r(fd, buf, sz);
        openpty(NULL, NULL, buf, &termios_buf, NULL);
        forkpty(NULL, buf, &termios_buf, NULL);
        login_tty(fd);
        
        // Old terminal interface
        gtty(fd, NULL);
        stty(fd, NULL);
        
        // Utmp functions
        setutent();
        getutent();
        endutent();
        getutid(&utmp_buf);
        getutline(&utmp_buf);
        pututline(&utmp_buf);
        getutent_r(&utmp_buf, &utmp_buf);
        getutid_r(&utmp_buf, &utmp_buf, &utmp_buf);
        getutline_r(&utmp_buf, &utmp_buf, &utmp_buf);
        utmpname("dummy");
        updwtmp("dummy", &utmp_buf);
        login(&utmp_buf);
        logout("tty1");
        logwtmp("tty1", "user", "host");
        
        // Utmpx functions
        setutxent();
        getutxent();
        endutxent();
        getutxid(&utmpx_buf);
        getutxline(&utmpx_buf);
        pututxline(&utmpx_buf);
        utmpxname("dummy");
        getutmp(&utmpx_buf, &utmp_buf);
        getutmpx(&utmp_buf, &utmpx_buf);
        
        // Resource usage
        getrusage(RUSAGE_SELF, NULL);
        getrlimit(RLIMIT_CPU, &rlim);
        getrlimit64(RLIMIT_CPU, (struct rlimit64*)&rlim);
        setrlimit(RLIMIT_CPU, &rlim);
        setrlimit64(RLIMIT_CPU, (struct rlimit64*)&rlim);
        vlimit(RLIMIT_CPU, 0);
        getpriority(PRIO_PROCESS, 0);
        setpriority(PRIO_PROCESS, 0, 0);
        nice(0);
        
        // System information
        get_nprocs_conf();
        get_nprocs();
        getloadavg(NULL, 1);
        getpagesize();
        get_phys_pages();
        get_avphys_pages();
        confstr(_CS_PATH, buf, sz);
        sysconf(_SC_OPEN_MAX);
        pathconf("dummy", _PC_NAME_MAX);
        fpathconf(fd, _PC_NAME_MAX);
        
        // Floating point environment
        fegetenv(&fenv);
        feholdexcept(&fenv);
        fesetenv(&fenv);
        feupdateenv(&fenv);
        fegetmode(&femode);
        fesetmode(&femode);
        feenableexcept(FE_DIVBYZERO);
        fedisableexcept(FE_DIVBYZERO);
        fegetexcept();
        feclearexcept(FE_ALL_EXCEPT);
        feraiseexcept(FE_DIVBYZERO);
        fesetexcept(FE_DIVBYZERO);
        fetestexcept(FE_DIVBYZERO);
        fegetexceptflag(&fexcept, FE_ALL_EXCEPT);
        fesetexceptflag(&fexcept, FE_ALL_EXCEPT);
        fetestexceptflag(&fexcept, FE_DIVBYZERO);
        fegetround();
        fesetround(FE_TONEAREST);
        
        // File truncation
        truncate("dummy", 0);
        truncate64("dummy", 0);
        ftruncate(fd, 0);
        ftruncate64(fd, 0);
        posix_fallocate(fd, 0, sz);
        posix_fallocate64(fd, 0, sz);
        
        // File synchronization
        sync();
        fsync(fd);
        fdatasync(fd);
        
        // Pipe functions
        pipe(NULL);
        popen("true", "r");
        pclose(fp);
        
        // I/O vector functions
        readv(fd, NULL, 0);
        writev(fd, NULL, 0);
        
        // I/O control
        ioctl(fd, 0);
        select(0, NULL, NULL, NULL, &tv);
        
        // File locking
        flockfile(fp);
        ftrylockfile(fp);
        funlockfile(fp);
        
        // Stream status
        __freadable(fp);
        __fwritable(fp);
        __freading(fp);
        __fwriting(fp);
        __flbf(fp);
        __fbufsize(fp);
        __fpending(fp);
        _flushlbf();
        __fpurge(fp);
        
        // Temporary files
        tmpfile();
        tmpfile64();
        tmpnam(buf);
        tmpnam_r(buf);
        tempnam("dummy", "prefix");
        mktemp(buf);
        mkstemp(buf);
        mkdtemp(buf);
        
        // Memory streams
        fmemopen(ptr, sz, "r");
        open_memstream(&str, &sz);
        
        // Cookie streams
        
        // Obstack functions
        obstack_alloc(&obs, sz);
        obstack_copy(&obs, ptr, sz);
        obstack_copy0(&obs, ptr, sz);
        obstack_free(&obs, ptr);
        obstack_blank(&obs, sz);
        obstack_grow(&obs, ptr, sz);
        obstack_grow0(&obs, ptr, sz);
        obstack_1grow(&obs, 'a');
        obstack_ptr_grow(&obs, ptr);
        obstack_int_grow(&obs, result);
        obstack_finish(&obs);
        obstack_object_size(&obs);
        obstack_room(&obs);
        obstack_1grow_fast(&obs, 'a');
        obstack_ptr_grow_fast(&obs, ptr);
        obstack_int_grow_fast(&obs, result);
        obstack_blank_fast(&obs, sz);
        obstack_base(&obs);
        obstack_next_free(&obs);
        obstack_printf(&obs, "test");
        obstack_vprintf(&obs, "test", ap);
        
        // Division functions
        div_result = div(10, 3);
        ldiv_result = ldiv(10L, 3L);
        lldiv_result = lldiv(10LL, 3LL);
        // imaxdiv_result = imaxdiv(imax, imax);
        
        // Searching and sorting
        qsort(ptr, sz, sz, cmp_fn);
        bsearch(ptr, ptr, sz, sz, cmp_fn);
        lfind(ptr, ptr, &sz, sz, cmp_fn);
        lsearch(ptr, ptr, &sz, sz, cmp_fn);
        
        // Tree search
        tsearch(ptr, &ptr, cmp_fn);
        tfind(ptr, &ptr, cmp_fn);
        tdelete(ptr, &ptr, cmp_fn);
        tdestroy(ptr, NULL);
        twalk(ptr, NULL);
        
        // Hash table
        hcreate(sz);
        hdestroy();
        hsearch(entry, ENTER);
        hcreate_r(sz, NULL);
        hdestroy_r(NULL);
        hsearch_r(entry, ENTER, &entry, NULL);
        
        // Pattern matching
        fnmatch("*", "test", 0);
        glob("*", 0, NULL, NULL);
        glob64("*", 0, NULL, NULL);
        globfree(&globbuf);
        globfree64(NULL);
        
        // Regular expressions
        regcomp(&regex, "test", 0);
        regexec(&regex, "test", 0, NULL, 0);
        regerror(0, &regex, buf, sz);
        regfree(&regex);
        
        // Word expansion
        wordexp("test", &we, 0);
        wordfree(&we);
        
        // Option parsing
        getopt(0, NULL, "abc");
        getopt_long(0, NULL, "abc", NULL, NULL);
        getopt_long_only(0, NULL, "abc", NULL, NULL);
        getsubopt(&str, NULL, &str);
        
        // Argument parsing
        argp_parse(&argp_buf, 0, NULL, 0, NULL, NULL);
        argp_usage(&argp_state);
        argp_error(&argp_state, "error");
        argp_failure(&argp_state, 1, 0, "failure");
        argp_state_help(&argp_state, fp, 0);
        argp_help(&argp_buf, fp, 0, "program");
        
        // Argument vectors
        argz_create(NULL, &str, &sz);
        argz_create_sep("test", ':', &str, &sz);
        argz_count(str, sz);
        argz_extract(str, sz, NULL);
        argz_stringify(str, sz, ':');
        argz_add(&str, &sz, "test");
        argz_add_sep(&str, &sz, "test", ':');
        argz_append(&str, &sz, "test", 4);
        argz_delete(&str, &sz, str);
        argz_insert(&str, &sz, str, "test");
        argz_next(str, sz, str);
        argz_replace(&str, &sz, "old", "new", NULL);
        
        // Environment vectors
        envz_entry(str, sz, "PATH");
        envz_get(str, sz, "PATH");
        envz_add(&str, &sz, "VAR", "value");
        envz_merge(&str, &sz, str, sz, 1);
        envz_strip(&str, &sz);
        envz_remove(&str, &sz, "VAR");
        
        // Error handling
        strerror(errno);
        strerror_r(errno, buf, sz);
        perror("error");
        error(0, errno, "error");
        error_at_line(0, errno, "file", 1, "error");
        warn("warning");
        vwarn("warning", ap);
        warnx("warning");
        vwarnx("warning", ap);
        err(1, "error");
        verr(1, "error", ap);
        errx(1, "error");
        verrx(1, "error", ap);
        
        // Syslog
        openlog("program", LOG_PID, LOG_USER);
        syslog(LOG_INFO, "message");
        vsyslog(LOG_INFO, "message", ap);
        setlogmask(LOG_UPTO(LOG_INFO));
        closelog();
        
        // Message formatting
        fmtmsg(0, "label", 0, "text", "action", "tag");
        addseverity(10, "CUSTOM");
        
        // Monetary formatting
        strfmon(buf, sz, "%n", 123.45);
        
        // Conversion functions
        ecvt(d, 2, &result, &result);
        fcvt(d, 2, &result, &result);
        gcvt(d, 2, buf);
        qecvt(ld, 2, &result, &result);
        qfcvt(ld, 2, &result, &result);
        qgcvt(ld, 2, buf);
        ecvt_r(d, 2, &result, &result, buf, sz);
        fcvt_r(d, 2, &result, &result, buf, sz);
        qecvt_r(ld, 2, &result, &result, buf, sz);
        qfcvt_r(ld, 2, &result, &result, buf, sz);
        strfromd(buf, sz, "%.2f", d);
        strfromf(buf, sz, "%.2f", f);
        strfroml(buf, sz, "%.2Lf", ld);
        
        // String transformation
        strcoll(buf, buf);
        wcscoll(wbuf, wbuf);
        strxfrm(buf, buf, sz);
        wcsxfrm(wbuf, wbuf, sz);
        
        // Memory information
        mallinfo();
        mallopt(M_TRIM_THRESHOLD, 0);
        mcheck(NULL);
        mprobe(ptr);
        mtrace();
        muntrace();
        
        // Encryption
        crypt(buf, buf);
        crypt_r(buf, buf, &crypt_data_buf);
        
        // Password entry
        getpass("Password:");
        
        // String utilities
        strfry(buf);
        
        // File tree walk
        
        // Base64-like encoding
        l64a(result);
        a64l(buf);
        
        // Timers
        setitimer(ITIMER_REAL, NULL, NULL);
        getitimer(ITIMER_REAL, NULL);
        alarm(10);
        
        // Jump functions
        setjmp(jbuf);
        longjmp(jbuf, 1);
        sigsetjmp(sjbuf, 1);
        siglongjmp(sjbuf, 1);
        
        // Context functions
        getcontext(&uctx);
        setcontext(&uctx);
        makecontext(&uctx, NULL, 0);
        swapcontext(&uctx, &uctx);
        
        // File system mounting
        mount("device", "mountpoint", "ext4", 0, NULL);
        umount("mountpoint");
        umount2("mountpoint", 0);
        
        // Mount table
        setmntent("/etc/mtab", "r");
        endmntent(fp);
        getmntent(fp);
        getmntent_r(fp, &mnt, buf, sz);
        addmntent(fp, &mnt);
        hasmntopt(&mnt, "ro");
        
        // File system information
        setfsent();
        endfsent();
        getfsent();
        getfsspec("device");
        getfsfile("mountpoint");
        
        // Character conversion
        iconv_open("UTF-8", "ISO-8859-1");
        iconv(cd, &str, &sz, &str, &sz);
        iconv_close(cd);
        
        // Asynchronous I/O
        aio_read(&aiocb_buf);
        aio_read64((struct aiocb64*)&aiocb_buf);
        aio_write(&aiocb_buf);
        aio_write64((struct aiocb64*)&aiocb_buf);
        aio_error(&aiocb_buf);
        aio_error64((struct aiocb64*)&aiocb_buf);
        aio_return(&aiocb_buf);
        aio_return64((struct aiocb64*)&aiocb_buf);
        aio_cancel(fd, &aiocb_buf);
        aio_cancel64(fd, (struct aiocb64*)&aiocb_buf);
        aio_suspend((const struct aiocb**)&ptr, 1, NULL);
        aio_suspend64((const struct aiocb64**)&ptr, 1, NULL);
        aio_fsync(O_SYNC, &aiocb_buf);
        aio_fsync64(O_SYNC, (struct aiocb64*)&aiocb_buf);
        lio_listio(LIO_WAIT, (struct aiocb**)&ptr, 1, NULL);
        lio_listio64(LIO_WAIT, (struct aiocb64**)&ptr, 1, NULL);
        aio_init(NULL);
        
        // Pthread functions
        pthread_key_create(&key, NULL);
        pthread_key_delete(key);
        pthread_getspecific(key);
        pthread_setspecific(key, ptr);
        pthread_getattr_default_np(NULL);
        pthread_setattr_default_np(NULL);
        
        // Semaphores
        sem_init(&sem, 0, 1);
        sem_destroy(&sem);
        sem_open("semaphore", O_CREAT, mode, 1);
        sem_close(&sem);
        sem_unlink("semaphore");
        sem_wait(&sem);
        sem_timedwait(&sem, NULL);
        sem_trywait(&sem);
        sem_post(&sem);
        sem_getvalue(&sem, &result);
        semctl(0, 0, IPC_STAT);
        semget(IPC_PRIVATE, 1, IPC_CREAT);
        semop(0, NULL, 0);
        semtimedop(0, NULL, 0, NULL);
        
        // System calls
        syscall(SYS_getpid);
        sysctl(NULL, 0, NULL, NULL, NULL, 0);
        
        // Backtrace
        backtrace(&ptr, 1);
        backtrace_symbols(&ptr, 1);
        backtrace_symbols_fd(&ptr, 1, fd);
        
        // Random entropy
        getentropy(ptr, sz);
        getrandom(ptr, sz, 0);
        
        // Auxiliary vector
        // Response matching
        rpmatch("yes");
        
        // Printf utilities
        parse_printf_format("test", 0, NULL);
        printf_size(fp, NULL, NULL);
        printf_size_info(NULL, 0, NULL);
        register_printf_function('X', NULL, NULL);
        
        // Wide character utilities
        fwide(fp, 0);
        
        // Math utilities
        canonicalize(&d, &d);
        canonicalizef(&f, &f);
        canonicalizel(&ld, &ld);
        getpayload(&d);
        getpayloadf(&f);
        getpayloadl(&ld);
        setpayload(&d, d);
        setpayloadf(&f, f);
        setpayloadl(&ld, ld);
        setpayloadsig(&d, d);
        setpayloadsigf(&f, f);
        setpayloadsigl(&ld, ld);
        fromfp(d, FE_TONEAREST, 32);
        fromfpf(f, FE_TONEAREST, 32);
        fromfpl(ld, FE_TONEAREST, 32);
        ufromfp(d, FE_TONEAREST, 32);
        ufromfpf(f, FE_TONEAREST, 32);
        ufromfpl(ld, FE_TONEAREST, 32);
        fromfpx(d, FE_TONEAREST, 32);
        fromfpxf(f, FE_TONEAREST, 32);
        fromfpxl(ld, FE_TONEAREST, 32);
        ufromfpx(d, FE_TONEAREST, 32);
        ufromfpxf(f, FE_TONEAREST, 32);
        ufromfpxl(ld, FE_TONEAREST, 32);
        // totalorder(d, d);
        totalorderf(NULL, NULL);
        totalorderl(NULL, NULL);
        totalordermag(NULL, NULL);
        totalordermagf(NULL, NULL);
        totalordermagl(NULL, NULL);
        
        // PowerPC specific functions
        __ppc_get_timebase();
        __ppc_get_timebase_freq();
        __ppc_yield();
        __ppc_mdoio();
        __ppc_mdoom();
        __ppc_set_ppr_med();
        __ppc_set_ppr_low();
        __ppc_set_ppr_med_low();
        __ppc_set_ppr_very_low();
        __ppc_set_ppr_med_high();
        
        // Directory type conversion
        IFTODT(mode);
        DTTOIF(DT_REG);
        
        // Utility functions
        basename("path/file");
        dirname("path/file");
        getlogin();
        cuserid(buf);
        
        // File duplication
        dup(fd);
        dup2(fd, fd);
        
        // Time utilities
        utime("file", NULL);
        utimes("file", NULL);
        lutimes("file", NULL);
        futimes(fd, NULL);
        
        // Signal utilities
        strsignal(SIGINT);
        psignal(SIGINT, "signal");
        
        // Math special cases
        sincos(d, &d, &d);
        sincosf(f, &f, &f);
        sincosl(ld, &ld, &ld);
        execv("dummy", NULL);
        execl("dummy", "dummy", NULL);
        execve("dummy", NULL, NULL);
        execle("dummy", "dummy", NULL, NULL);
        execvp("dummy", NULL);
        execlp("dummy", "dummy", NULL);
        exit(0);
        _exit(0);
        _Exit(0);
        abort();
        atexit(NULL);
        on_exit(NULL, NULL);
}

void __attribute__((section(".fun"))) xd() {
    exit(0);  // Function never called, but exit still imported
}
