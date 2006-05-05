#ifndef ASF_CONVERT_H
#define ASF_CONVERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char *in_name;          // input file name
  char *out_name;         // output file name
  int import;             // import flag
  int sar_processing;     // SAR processing flag
  int image_stats;        // image stats flag (for internal use only)
  int detect_cr;          // detect corner reflector flag (for internal use only)
  int terrain_correct;    // terrain correction flag
  int geocoding;          // geocoding flag
  int export;             // export flag
  int intermediates;      // flag to keep intermediates
  int quiet;              // quiet flag
  int short_config;       // short configuration file flag;
  char *defaults;         // default values file
  char *batchFile;        // batch file name
  char *prefix;           // prefix for output file naming scheme
  char *suffix;           // suffix for output file naming scheme
} s_general;

typedef struct
{
  char *format;           // input format: CEOS, STF, ASF
  char *radiometry;       // data type: AMPLITUDE_IMAGE, 
                          // POWER_IMAGE, 
                          // SIGMA_IMAGE, 
                          // GAMMA_IMAGE,
                          // BETA_IMAGE
  char *lut;              // look up table file name (CIS only)
  double lat_begin;       // latitude constraint begin
  double lat_end;         // latitude constraint end
  char *prc;              // precision state vector location (to be implemented)
} s_import;

typedef struct
{
  char *radiometry;       // data type: AMPLITUDE_IMAGE,
                          // POWER_IMAGE,
                          // SIGMA_IMAGE,
                          // GAMMA_IMAGE,
                          // BETA_IMAGE
} s_sar_processing;

typedef struct
{
  char *values;           // value axis: LOOK, INCIDENCE, RANGE
  int bins;               // number of bins
  double interval;        // interval between bins
} s_image_stats;

typedef struct
{
  char *cr_location;      // file with corner reflector locations
  int chips;              // flag to save image analysis chips as binary file
  int text;               // flag to save image analysis chips as text file
} s_detect_cr;

typedef struct
{
  double pixel;           // pixel size for terrain corrected product
  char *dem;              // reference DEM file name
} s_terrain_correct;

typedef struct
{
  char *projection;       // projection parameters file
  double pixel;           // pixel size for geocoded product
  double height;          // average height of the data
  char *datum;            // datum: WGS84, NAD27, NAD83
  char *resampling;       // resampling method: NEAREST_NEIGHBOR, BILINEAR, BICUBIC
  int force;              // force flag
} s_geocoding;

typedef struct
{
  char *format;           // output format: ASF, GEOTIFF, JPEG, PPM
  char *byte;             // conversion to byte: SIGMA, MINMAX, TRUNCATE, 
                          // HISTOGRAM_EQUALIZE
} s_export;

typedef struct
{
  char comment[255];                   // first line for comments
  s_general *general;                  // general processing details
  s_import *import;                    // importing parameters
  s_sar_processing *sar_processing;    // SAR processing parameters
  s_image_stats *image_stats;          // image stats parameters
  s_detect_cr *detect_cr;              // corner reflector detection parameters
  s_terrain_correct *terrain_correct;  // terrain correction parameters
  s_geocoding *geocoding;              // geocoding parameters
  s_export *export;                    // exporting parameters
} convert_config;

// checking return values in the main program
void check_return(int ret, char *msg);

// configuration functions
int strindex(char s[], char t[]);
char *read_param(char *line);
char *read_str(char *line, char *param);
int read_int(char *line, char *param);
double read_double(char *line, char *param);
int init_convert_config(char *configFile);
convert_config *init_fill_convert_config(char *configFile);
convert_config *read_convert_config(char *configFile);
int write_convert_config(char *configFile, convert_config *cfg);

// function call definitions
int exit_code;

char *str2upper(char *string);
//int asf_import(char *inFile, char *outFile, char *format, char *radiometry,
//               char *prcOrbits, double lat_begin, double lat_end);
int ardop(char *options, char *inFile, char *outFile);
int image_stats(char *inFile, char *outFile, char *values, int bins, 
                double interval);
int detect_cr(char *inFile, char *crFile, char *outFile, int chips, int text);
//int asf_terrcorr(char *options, char *inFile, char *demFile, char *outFile);
//int asf_geocode(char *options, char *inFile, char *outFile);
//int asf_export(char *options, char *inFile, char *outFile);
int call_asf_convert(char *configFile);

#endif
