CFLAGS += $(HDF5_CFLAGS)
CFLAGS += $(GEOTIFF_CFLAGS)
CFLAGS += $(HDF5_CFLAGS)
# Makefile for		: create_dem_grid

include ../../make_support/system_rules

LIBS  = \
	$(LIBDIR)/libasf_sar.a \
	$(LIBDIR)/asf_meta.a \
	$(GSL_LIBS) \
	$(LIBDIR)/libasf_proj.a \
	$(LIBDIR)/asf.a \
	$(PROJ_LIBS) \
	$(XML_LIBS) \
	-lm
OBJS = create_dem_grid.o

CFLAGS += -Wall $(GLIB_CFLAGS)

all: create_dem_grid
	rm *.o

create_dem_grid: $(OBJS)
	$(CC) $(CFLAGS) -o create_dem_grid $(OBJS) $(LIBS) $(LDFLAGS)
	mv create_dem_grid$(BIN_POSTFIX) $(BINDIR)
	cp create_dem_grid.1 ../../man/cat1

clean:
	rm -f core ${OBJS} *.o
