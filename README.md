# STPM3x
RIOT OS driver for STPM3x
Tested on RIOT 2018.10 with STPM33 only with current values reading on channel 1.

## Installation
If you want to use this driver, add in file :

> /sys/auto_init/auto_init.c b/sys/auto_init/auto_init.c

this :
```
#ifdef MODULE_STPM3X
    extern void auto_init_stpm3x(void);
    auto_init_stpm3x();
#endif
```
and add the driver to drivers Makefile.include :
> /drivers/Makefile.include b/drivers/Makefile.include

with
```
ifneq (,$(filter stpm3x,$(USEMODULE)))
  USEMODULE_INCLUDES += $(RIOTBASE)/drivers/stpm3x/include
endif
```

and move the `auto_init_stpm3x.c` file to `/sys/auto_init/saul/` + the `stpm3x.h` file to `/drivers/include`

All other files have to be in `/drivers/` in a folder named `stpm3x`