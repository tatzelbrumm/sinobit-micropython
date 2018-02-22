ECHO = @echo
PYTHON = python

QSTR_DEFS = inc/py/qstrdefs.h inc/microbit/qstrdefsport.h

OUT_SRC = build/bbc-microbit-classic-gcc-nosd/source/sinobit-micropython
HEX_SRC = $(OUT_SRC).hex
HEX_FINAL = build/firmware.hex

all: yotta

yotta: inc/genhdr/qstrdefs.generated.h inc/sinobit/zpixfont.h
	@yt build
	@/bin/cp $(HEX_SRC) $(HEX_FINAL)
	@arm-none-eabi-size $(OUT_SRC)

inc/sinobit/zpixfont.h: config.json
	$(ECHO) "Generating zpixfont with cog..."
	@cog.py -r -D config="./config.json" -I ./tools/ ./inc/sinobit/zpixfont.h

# Note: we need to protect the qstr names from the preprocessor, so we wrap
# the lines in "" and then unwrap after the preprocessor is finished.
inc/genhdr/qstrdefs.generated.h: $(QSTR_DEFS) tools/makeqstrdata.py inc/microbit/mpconfigport.h inc/py/mpconfig.h
	$(ECHO) "Generating $@"
	@cat $(QSTR_DEFS) | sed 's/^Q(.*)/"&"/' | $(CPP) -E -Iinc -Iinc/microbit - | sed 's/^"\(Q(.*)\)"/\1/' > build/qstrdefs.preprocessed.h
	@$(PYTHON) tools/makeqstrdata.py build/qstrdefs.preprocessed.h > $@

deploy: $(HEX_FINAL)
	$(ECHO) "Deploying $<"
	@mount /dev/sdb
	@sleep 1s
	@cp $< /mnt/
	@sleep 1s
	@umount /mnt

serial:
	@picocom /dev/ttyACM0 -b 115200
