#include <ctype.h>
#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

#include <asf.h>
#include <asf_endian.h>
#include <asf_meta.h>
#include <asf_raster.h>
#include <asf_contact.h>
#include <asf_license.h>
#include <asf_sar.h>
#include <poly.h>
#include <seedsquares.h>

#include <asf_terrcorr.h>

#define MASK_SEED_POINTS 20
#define PAD 200

char *strdup(const char *);

static int int_rnd(double x)
{
  return (int)floor(x+0.5);
}

static void ensure_ext(char **filename, const char *ext)
{
  char *ret = MALLOC(sizeof(char)*(strlen(*filename)+strlen(ext)+5));
  strcpy(ret, *filename);

  // blow away current extension if necessary
  char *p = findExt(ret);
  if (p) *p = '\0';

  if (ext[0] != '.') strcat(ret, ".");
  strcat(ret, ext);

  free(*filename);
  *filename = ret;
}

static void remove_file(const char * file)
{
  if (fileExists(file)) {
    unlink(file);
  }
}

static char * appendSuffix(const char *inFile, const char *suffix)
{
  char *suffix_pid = MALLOC(sizeof(char)*(strlen(suffix)+25));
  sprintf(suffix_pid, "%s", suffix);

  char * ret = appendToBasename(inFile, suffix_pid);

  free(suffix_pid);
  return ret;
}

static char *outputName(const char *dir, const char *base, const char *suffix)
{
    int dirlen = strlen(dir);
    int baselen = strlen(base);
    int len = dirlen > baselen ? dirlen : baselen;

    char *fil = MALLOC(sizeof(char)*(dirlen+baselen+strlen(suffix)+10));
    char *dirTmp = MALLOC(sizeof(char) * (len+10));
    char *fileTmp = MALLOC(sizeof(char) * (len+10));

    split_dir_and_file(dir, dirTmp, fileTmp);
    strcpy(fil, dirTmp);

    split_dir_and_file(base, dirTmp, fileTmp);
    strcat(fil, fileTmp);

    char *ret = appendSuffix(fil, suffix);

    free(fil);
    free(dirTmp);
    free(fileTmp);

    return ret;
}

// attempt to remove "<file>.img" and "<file>.meta", etc files
static void clean(const char *file)
{
    if (file)
    {
        char * img_file = appendExt(file, ".img");
        char * meta_file = appendExt(file, ".meta");
        char * ddr_file = appendExt(file, ".ddr");
        
        remove_file(img_file);
        remove_file(meta_file);
        remove_file(ddr_file);
        remove_file(file);
        
        free(img_file);
        free(meta_file);
        free(ddr_file);
    }
}

static void 
fftMatchQ(char *file1, char *file2, float *dx, float *dy, float *cert)
{
  int qf_saved = quietflag;
  quietflag = 1;
  fftMatch(file1, file2, NULL, dx, dy, cert);
  quietflag = qf_saved;
}

static int mini(int a, int b)
{
  return a < b ? a : b;
}

static void 
fftMatch_atCorners(char *output_dir, char *sar, char *dem, const int size)
{
  float dx_ur, dy_ur; 
  float dx_ul, dy_ul; 
  float dx_lr, dy_lr; 
  float dx_ll, dy_ll; 
  float cert;
  double rsf, asf;

  char *chopped_sar, *chopped_dem;
  int nl, ns;
  meta_parameters *meta_sar, *meta_dem;
  long long lsz = (long long)size;

  meta_sar = meta_read(sar);
  meta_dem = meta_read(dem);

  chopped_sar = outputName(output_dir, sar, "_chip");
  chopped_dem = outputName(output_dir, dem, "_chip");

  nl = mini(meta_sar->general->line_count, meta_dem->general->line_count);
  ns = mini(meta_sar->general->sample_count, meta_dem->general->sample_count);

  meta_free(meta_dem);

  // Require the image be 4x the chip size in each direction, otherwise
  // the corner matching isn't really that meaningful
  if (nl < 4*size || ns < 4*size) {
    asfPrintStatus("Image is too small, skipping corner matching.\n");
    return;
  }

  trim(sar, chopped_sar, (long long)0, (long long)0, lsz, lsz);
  trim(dem, chopped_dem, (long long)0, (long long)0, lsz, lsz);

  fftMatchQ(chopped_sar, chopped_dem, &dx_ur, &dy_ur, &cert);
  asfPrintStatus("UR: %14.10f %14.10f %14.10f\n", dx_ur, dy_ur, cert);

  trim(sar, chopped_sar, (long long)(ns-size), (long long)0, lsz, lsz);
  trim(dem, chopped_dem, (long long)(ns-size), (long long)0, lsz, lsz);

  fftMatchQ(chopped_sar, chopped_dem, &dx_ul, &dy_ul, &cert);
  asfPrintStatus("UL: %14.10f %14.10f %14.10f\n", dx_ul, dy_ul, cert);

  trim(sar, chopped_sar, (long long)0, (long long)(nl-size), lsz, lsz);
  trim(dem, chopped_dem, (long long)0, (long long)(nl-size), lsz, lsz);

  fftMatchQ(chopped_sar, chopped_dem, &dx_lr, &dy_lr, &cert);
  asfPrintStatus("LR: %14.10f %14.10f %14.10f\n", dx_lr, dy_lr, cert);

  trim(sar, chopped_sar, (long long)(ns-size), (long long)(nl-size),
       lsz, lsz);
  trim(dem, chopped_dem, (long long)(ns-size), (long long)(nl-size),
       lsz, lsz);

  fftMatchQ(chopped_sar, chopped_dem, &dx_ll, &dy_ll, &cert);
  asfPrintStatus("LL: %14.10f %14.10f %14.10f\n", dx_ll, dy_ll, cert);

  asfPrintStatus("Range shift: %14.10lf top\n", (double)(dx_ul-dx_ur));
  asfPrintStatus("             %14.10lf bottom\n", (double)(dx_ll-dx_lr));
  asfPrintStatus("   Az shift: %14.10lf left\n", (double)(dy_ul-dy_ll));
  asfPrintStatus("             %14.10lf right\n\n", (double)(dy_ur-dy_lr));

  nl = meta_sar->general->line_count;
  ns = meta_sar->general->sample_count;

  rsf = 1 - (fabs((double)(dx_ul-dx_ur)) + fabs((double)(dx_ll-dx_lr)))/ns/2;
  asf = 1 - (fabs((double)(dy_ul-dy_ll)) + fabs((double)(dy_ur-dy_lr)))/nl/2;

  asfPrintStatus("Suggested scale factors: %14.10lf range\n", rsf);
  asfPrintStatus("                         %14.10lf azimuth\n\n", asf);

  meta_free(meta_sar);

  clean(chopped_sar);
  clean(chopped_dem);
  free(chopped_sar);
  free(chopped_dem);
}

int asf_terrcorr(char *sarFile, char *demFile, char *userMaskFile,
		 char *outFile, double pixel_size)
{
  int do_fftMatch_verification = TRUE;
  int do_corner_matching = TRUE;
  int do_resample = TRUE;
  int do_interp = FALSE;
  int clean_files = TRUE;
  int dem_grid_size = 20;
  int do_terrain_correction = TRUE;
  int madassap = FALSE; // mask and dem are same size and projection
  int maskfill = 2;

  return asf_terrcorr_ext(sarFile, demFile, userMaskFile, outFile, pixel_size,
			  clean_files, do_resample, do_corner_matching,
                          do_interp, do_fftMatch_verification,
                          dem_grid_size, do_terrain_correction, maskfill,
                          madassap);
}

int refine_geolocation(char *sarFile, char *demFile, char *userMaskFile, 
                       char *outFile, int update_flag)
{
  double pixel_size = -1;
  int dem_grid_size = 20;
  int clean_files = TRUE;
  int do_resample = TRUE;
  int do_interp = FALSE;
  int do_fftMatch_verification = TRUE;
  int do_corner_matching = FALSE;
  int do_terrain_correction = FALSE;
  int madassap = FALSE; // mask and dem are same size and projection
  int ret;
  int maskfill = 2;
  ret = asf_terrcorr_ext(sarFile, demFile, userMaskFile, outFile, pixel_size,
                         clean_files, do_resample, do_corner_matching,
                         do_interp, do_fftMatch_verification, dem_grid_size,
                         do_terrain_correction, maskfill, madassap);

  if (ret==0)
  {
      // asf_terrcorr_ext with do_terrain_correction turned off just 
      // creates a metadata file - the actual image file is unchanged.
      if (update_flag) {
          // If the user issued the "update" flag, then we need to move
          // the new metadata file over the old metadata file.  (ie., we
          // are "updating" the existing metadata with the new shift values
          char *inMetaFile = appendExt(sarFile, ".meta");
          char *outMetaFile = appendExt(outFile, ".meta");
          fileCopy(outMetaFile, inMetaFile);
          clean(outFile);
          FREE(inMetaFile);
          FREE(outMetaFile);
      } else {
          // Otherwise, make a copy of the original image with the new name
          char *inImgFile = appendExt(sarFile, ".img");
          char *outImgFile = appendExt(outFile, ".img");
          fileCopy(inImgFile, outImgFile);
          FREE(inImgFile);
          FREE(outImgFile);
      }
  }

  return ret;
}

static char * getOutputDir(char *outFile)
{
    char *d = MALLOC(sizeof(char) * (strlen(outFile)+2));
    char *f = MALLOC(sizeof(char) * (strlen(outFile)+2));
    split_dir_and_file(outFile, d, f);
    free(f);
    return d;
}

static void
clip_dem(meta_parameters *metaSAR,
         char *srFile,
         char *demFile,   // we call this the DEM, but it could be a mask
         char *demClipped,
         char *what,      // "DEM" or "User Mask" probably
         char *otherFile, // file same size/projection as DEM to clip, or NULL
         char *otherClipped,
         char *otherWhat, // "User Mask" probably, or NULL
         char *output_dir,
         int dem_grid_size,
         int clean_files,
         int *p_demHeight)
{
    char *demGridFile = NULL;
    int polyOrder = 5;
    double coverage_pct;
    int demWidth, demHeight;

    asfPrintStatus("Now clipping %s: %s\n", what, demFile);

    // Generate a point grid for the DEM extraction.
    // The width and height of the grid is defined in slant range image
    // coordinates, while the grid is actually calculated in DEM space.
    // There is a buffer of 400 pixels in far range added to have enough
    // DEM around when we get to correcting the terrain.
    asfPrintStatus("Generating %dx%d %s grid...\n",
		   dem_grid_size, dem_grid_size, what);

    demGridFile = outputName(output_dir, srFile, "_demgrid");
    create_dem_grid_ext(demFile, srFile, demGridFile,
			metaSAR->general->sample_count,
			metaSAR->general->line_count, dem_grid_size,
			&coverage_pct);
  
    if (coverage_pct <= 0) {
      asfPrintError("%s and SAR images do not overlap!\n", what);
    } else if (coverage_pct <= 25) {
      asfPrintError("Insufficient %s coverage!\n", what);
    } else if (coverage_pct <= 99) {
      asfPrintWarning("Incomplete %s coverage!\n", what);
    }
    
    // Fit a fifth order polynomial to the grid points.
    // This polynomial is then used to extract a subset out of the reference 
    // DEM.
    asfPrintStatus("Fitting order %d polynomial to %s...\n", polyOrder, what);
    double maxErr;
    poly_2d *fwX, *fwY, *bwX, *bwY;
    fit_poly(demGridFile, polyOrder, &maxErr, &fwX, &fwY, &bwX, &bwY);
    asfPrintStatus("Maximum error in polynomial fit: %g.\n", maxErr);
    
    // Here is the actual work done for cutting out the DEM.
    // The adjustment of the DEM width by 400 pixels (originated in
    // create_dem_grid) needs to be factored in.
    
    demWidth = metaSAR->general->sample_count + DEM_GRID_RHS_PADDING;
    demHeight = metaSAR->general->line_count;
    
    asfPrintStatus("Clipping %s to %dx%d LxS using polynomial fit...\n",
		   what, demHeight, demWidth);
    
    remap_poly(fwX, fwY, bwX, bwY, demWidth, demHeight, demFile, demClipped);

    if (otherFile) {
        asfRequire(otherClipped && otherWhat, "required arguments were NULL");

        asfPrintStatus("Clipping %s using same clipping parameters as %s.\n",
                       otherWhat, what);
        asfPrintStatus("%s file: %s -> %s\n", otherWhat, otherFile,
                       otherClipped);
        asfPrintStatus("Clipping %s to %dx%d LxS using polynomial fit...\n",
                       otherWhat, demHeight, demWidth);

        remap_poly(fwX, fwY, bwX, bwY, demWidth, demHeight, otherFile,
                   otherClipped);
    }

    // finished with polynomials
    poly_delete(fwX);
    poly_delete(fwY);
    poly_delete(bwX);
    poly_delete(bwY);

    if (clean_files) {
        clean(demGridFile);
    }

    if (p_demHeight)
        *p_demHeight = demHeight;

    FREE(demGridFile);
}

static 
int match_dem(meta_parameters *metaSAR,
              char *sarFile,
              char *demFile, 
	      char *srFile,
              char *output_dir,
              char *userMaskFile,
	      char *demTrimSimAmp,
              char *demTrimSlant,
              char *userMaskClipped,
              int dem_grid_size, 
	      int do_corner_matching,
              int do_fftMatch_verification, 
              int do_refine_geolocation,
              int do_trim_slant_range_dem,
	      int apply_dem_padding,
              int mask_dem_same_size_and_projection,
              int clean_files,
              double *t_offset, 
	      double *x_offset)
{
  char *demClipped = NULL, *demSlant = NULL;
  char *demSimAmp = NULL;
  int num_attempts = 0;
  const float required_match = 2.5;
  double t_off, x_off;
  int redo_clipping;
  int demHeight;
  float dx, dy, cert;
  int idx, idy;

  // do-while that will repeat the dem grid generation and the fftMatch
  // of the sar & simulated sar, until the fftMatch doesn't turn up a
  // big offset.
  do
  {

    ++num_attempts;    
    asfPrintStatus("Using DEM '%s' for refining geolocation ...\n", demFile);

    demClipped = outputName(output_dir, demFile, "_clip");

    // Clip the DEM to the same size as the SAR image.  If a user mask was
    // provided, we must clip that one, too.
    if (userMaskFile) {
        if (mask_dem_same_size_and_projection) {
            // clip DEM & Mask at the same time, they'll use the same
            // clipping parameters
            clip_dem(metaSAR, srFile, demFile, demClipped, "DEM",
                     userMaskFile, userMaskClipped, "User Mask",
                     output_dir, dem_grid_size, clean_files, &demHeight);
        } else {
            // clip DEM & Mask separately
            clip_dem(metaSAR, srFile, demFile, demClipped, "DEM",
                     NULL, NULL, NULL, output_dir, dem_grid_size,
                     clean_files, &demHeight);
            clip_dem(metaSAR, srFile, userMaskFile, userMaskClipped,
                     "User Mask", NULL, NULL, NULL, output_dir, dem_grid_size, 
                     clean_files, NULL);
        }
    } else {
        // no user mask - just clip the DEM and we're good
        clip_dem(metaSAR, srFile, demFile, demClipped, "DEM",
                 NULL, NULL, NULL, output_dir, dem_grid_size, clean_files,
                 &demHeight);
    }
    
    // Generate a slant range DEM and a simulated amplitude image.
    asfPrintStatus("Generating slant range DEM and "
		   "simulated sar image...\n");
    demSlant = outputName(output_dir, demFile, "_slant");
    demSimAmp = outputName(output_dir, demFile, "_sim_amp");
    reskew_dem(srFile, demClipped, demSlant, demSimAmp, userMaskClipped);
    
    // Resize the simulated amplitude to match the slant range SAR image.
    asfPrintStatus("Resizing simulated sar image...\n");
    trim(demSimAmp, demTrimSimAmp, 0, 0, metaSAR->general->sample_count,
	 demHeight);
    
    asfPrintStatus("Determining image offsets...\n");
    if (userMaskFile) {
      /* OK now if we have a mask we need to find square patches that 
	 can be fftMatched without running into the mask.
	 then we average them all back together. to get the offset
      */
      int x_pos_list[MASK_SEED_POINTS];
      int y_pos_list[MASK_SEED_POINTS];
      int size_list[MASK_SEED_POINTS];
      long clipped_pixels[MASK_SEED_POINTS];
      int y;
      long long xpltl,ypltl,szl,xplbr,yplbr,mx,my;
      FILE *inseedmask;
      meta_parameters *maskmeta;
      char *demTrimSimAmp_ffft=NULL,  *srTrimSimAmp=NULL;
      float *mask;
      inseedmask = fopenImage(userMaskClipped,"rb");
      maskmeta = meta_read(userMaskClipped);
      mask = (float *) MALLOC(sizeof(float) * metaSAR->general->sample_count 
			      * demHeight);
      for (y=0;y<demHeight;y++) // read in the whole mask image
	get_float_line(inseedmask, maskmeta, y,
                       mask + y * metaSAR->general->sample_count);
      FCLOSE(inseedmask);
      lay_seeds(MASK_SEED_POINTS,mask,metaSAR->general->sample_count, 
                demHeight, x_pos_list, y_pos_list, size_list, clipped_pixels);
      FREE(mask);
      szl = 0;
      for(y=1;y<MASK_SEED_POINTS;y++)
	{
	  if ( abs(clipped_pixels[y]) > szl)
	    {
	      szl = abs(clipped_pixels[y]);
	      mx = x_pos_list[y];
	      my = y_pos_list[y];
	      xpltl = x_pos_list[y]-(size_list[y]);
	      ypltl = y_pos_list[y]-(size_list[y]);
	      xplbr = x_pos_list[y]+(size_list[y]);
	      yplbr = y_pos_list[y]+(size_list[y]);
	      
	    }
	}
      
      if (xpltl < 0 ) xpltl = 0;
      if (xpltl >= metaSAR->general->sample_count ) 
	xpltl = metaSAR->general->sample_count-1;
      if (ypltl < 0 ) ypltl = 0;
      if (ypltl >= demHeight ) 
	ypltl = demHeight-1;			
      if (xplbr < 0 ) xplbr = 0;
      if (xplbr >= metaSAR->general->sample_count ) 
	xplbr = metaSAR->general->sample_count-1;
      if (yplbr < 0 ) yplbr = 0;
      if (yplbr >= demHeight ) 
	yplbr = demHeight-1;			
      
      printf( " clipping region is %lld %lld %lld %lld \n"
	      " which means it is %lld wide by %lld high \n"
	      " with a mid point %lld %lld size %lld (clipped pixels) \n",
	      xpltl,ypltl,xplbr,yplbr,xplbr-xpltl,yplbr-ypltl,mx,my,szl );	
      
      demTrimSimAmp_ffft = outputName(output_dir, demFile, "_sim_amp_trim_for_fft");
      srTrimSimAmp = outputName(output_dir, srFile, "_src_trim_for_fft");
      
      asfPrintStatus(" creating trimmed regions \n %s \n %s \n ", 
		     demTrimSimAmp_ffft, srTrimSimAmp);
      trim(demTrimSimAmp, demTrimSimAmp_ffft,xpltl, ypltl,  xplbr-xpltl, 
	   yplbr-ypltl) ;
      trim(srFile, srTrimSimAmp, xpltl, ypltl,  xplbr-xpltl, yplbr-ypltl);
      asfPrintStatus(" passed in %lld %lld %lld %lld \n",
		     xpltl, ypltl,xplbr-xpltl, yplbr-ypltl);
      fftMatchQ(srTrimSimAmp, demTrimSimAmp_ffft, &dx, &dy, &cert);
      meta_free(maskmeta);
    }
    else {
      // Match the real and simulated SAR image to determine the offset.
      // Read the offset out of the offset file.
      fftMatchQ(srFile, demTrimSimAmp, &dx, &dy, &cert);
    }
    
    asfPrintStatus("Correlation (cert=%5.2f%%): dx=%f dy=%f\n",
		   100*cert, dx, dy);
    
    idx = - int_rnd(dx);
    idy = - int_rnd(dy);
    
    redo_clipping = FALSE;

    if (fabs(dy) > required_match || fabs(dx) > required_match || 
	do_refine_geolocation)
    {
	// The fftMatch resulted in a large offset

	// This means we very likely did not clip the right portion of
	// the DEM.  So, shift the slant range image and re-clip.

	if (num_attempts >= 3)
        {
            // After 3 tries, we must not be getting good results from
            // fftMatch, since the offsets are clearly bogus.
	    asfPrintWarning("Could not resolve offset!\n"
			    "Continuing ... however your result may "
                            "be incomplete and/or incorrect.\n");

            if (cert<.5) {
                asfPrintStatus("You may get better results by supplying "
                               "a mask file, to mask out regions\n"
                               "which provide poor matching, such as "
                               "water or glaciers.\n");
            }

	    break;
        }
	else
        {
	    // Correct the metadata of the SAR image for the offsets 
	    // that we found
	    asfPrintStatus("Adjusting metadata to account for offsets...\n");
	    refine_offset(dx, dy, metaSAR, &t_off, &x_off);
	    asfPrintStatus("  Time Shift: %f -> %f\n"
			   "  Slant Shift: %f -> %f\n",
			   metaSAR->sar->time_shift,
			   metaSAR->sar->time_shift + t_off,
			   metaSAR->sar->slant_shift,
			   metaSAR->sar->slant_shift + x_off);
	    metaSAR->sar->time_shift += t_off;
	    metaSAR->sar->slant_shift += x_off;
	    meta_write(metaSAR, srFile);
	    
	    if (!do_refine_geolocation) {
	      asfPrintStatus("Found a large offset (%dx%d LxS pixels)\n"
			     "Adjusting SAR image and re-clipping DEM.\n",
			     idy, idx);

	      redo_clipping = TRUE;
            }
        }
    }
    
  } while (redo_clipping);
  
  // Corner test
  if (do_corner_matching) {
    int chipsz = 256;
    asfPrintStatus("Doing corner fftMatching... (using %dx%d chips)\n",
		   chipsz, chipsz);
    fftMatch_atCorners(output_dir, srFile, demTrimSimAmp, chipsz);
  }
  
  // Apply the offset to the simulated amplitude image.
  asfPrintStatus("Applying offsets to simulated sar image...\n");
  trim(demSimAmp, demTrimSimAmp, idx, idy, metaSAR->general->sample_count,
       demHeight);
  
  // Verify that the applied offset in fact does the trick.
  if (do_fftMatch_verification) {
      float dx2, dy2;
      
      asfPrintStatus("Verifying offsets are now close to zero...\n");
      fftMatchQ(srFile, demTrimSimAmp, &dx2, &dy2, &cert);
      
      asfPrintStatus("Correlation after shift (cert=%5.2f%%): "
                     "dx=%f dy=%f\n", 
                     100*cert, dx2, dy2);
      
      double match_tolerance = 1.0;
      if (sqrt(dx2*dx2 + dy2*dy2) > match_tolerance) {
          asfPrintError("Correlated images failed to match!\n"
                        " Original fftMatch offset: "
                        "(dx,dy) = %14.9f,%14.9f\n"
                        "   After shift, offset is: "
                            "(dx,dy) = %14.9f,%14.9f\n",
                        dx, dy, dx2, dy2);
      }
  }

  if (do_trim_slant_range_dem)
  {      
      // Apply the offset to the slant range DEM.
      asfPrintStatus("Applying offsets to slant range DEM...\n");

      int width = metaSAR->general->sample_count;
      if (apply_dem_padding)
          width += PAD;

      trim(demSlant, demTrimSlant, idx, idy, width, demHeight);
  }

  if (clean_files) {
    clean(demClipped);
    clean(demSimAmp);
    clean(demSlant);
  }

  FREE(demClipped);
  FREE(demSimAmp);
  FREE(demSlant);

  *t_offset = t_off;
  *x_offset = x_off;

  return 0;

}

int asf_check_geolocation(char *sarFile, char *demFile, char *userMaskFile,
			  char *simAmpFile, char *demSlant)
{
  int do_corner_matching = TRUE;
  int do_fftMatch_verification = TRUE;
  int do_refine_geolocation = TRUE;
  int do_trim_slant_range_dem = TRUE;
  int apply_dem_padding = FALSE;
  int clean_files = TRUE;
  int madssap = FALSE; // mask and dem same size and projection
  int dem_grid_size = 20;
  double t_offset, x_offset;
  char output_dir[255];
  char *userMaskClipped = NULL;
  meta_parameters *metaSAR;
  strcpy(output_dir, "");

  if (userMaskFile)
      userMaskClipped = outputName("", userMaskFile, "_clip");

  metaSAR = meta_read(sarFile);
  match_dem(metaSAR, sarFile, demFile, sarFile, output_dir, userMaskFile, 
	    simAmpFile, demSlant, userMaskClipped, dem_grid_size,
            do_corner_matching, do_fftMatch_verification,
            do_refine_geolocation, do_trim_slant_range_dem, apply_dem_padding,
            madssap, clean_files, &t_offset, &x_offset);

  if (clean_files)
      clean(userMaskClipped);

  return 0;
}

int asf_terrcorr_ext(char *sarFile, char *demFile, char *userMaskFile,
		     char *outFile, double pixel_size, int clean_files,
		     int do_resample, int do_corner_matching, int do_interp,
		     int do_fftMatch_verification, int dem_grid_size,
		     int do_terrain_correction, int maskfill,
                     int mask_and_dem_are_same_size_and_projection)
{
  char *resampleFile = NULL, *srFile = NULL, *resampleFile_2 = NULL;
  char *demTrimSimAmp = NULL, *demTrimSlant = NULL;
  char *lsMaskFile, *padFile = NULL, *userMaskClipped = NULL;
  char *deskewDemFile = NULL, *deskewDemMask = NULL;
  char *output_dir;
  double demRes, sarRes, maskRes;
  meta_parameters *metaSAR, *metaDEM, *metamask;
  int force_resample = FALSE;
  double t_offset, x_offset;
  int madssap = mask_and_dem_are_same_size_and_projection;

  // we want passing in an empty string for the mask to mean "no mask"
  if (userMaskFile && strlen(userMaskFile) == 0)
      userMaskFile = NULL;

  asfPrintStatus("Reading metadata...\n");
  metaSAR = meta_read(sarFile);
  metaDEM = meta_read(demFile);
  if (userMaskFile)
      metamask = meta_read(userMaskFile);
  
  output_dir = getOutputDir(outFile);

  // Warning regarding square pixels in the DEM.  Still not sure
  // exactly what effects this could have
  if (metaDEM->general->x_pixel_size != metaDEM->general->y_pixel_size) {
    asfPrintStatus("DEM does not have square pixels!\n"
		   "x pixel size = %gm, y pixel size = %gm.\n",
	metaDEM->general->x_pixel_size, metaDEM->general->y_pixel_size);
    asfPrintStatus("Results may not be as expected.\n");
  }

  demRes = metaDEM->general->x_pixel_size;
  sarRes = metaSAR->general->x_pixel_size;
  if (userMaskFile)
  {
  	maskRes = metamask->general->x_pixel_size; 
        userMaskClipped = outputName(output_dir, userMaskFile, "_clip");
  }

  // Check if the user requested a pixel size that requires
  // too much oversampling.
  if (pixel_size > 0 && pixel_size < sarRes / 2) {
      asfPrintWarning(
	"The requested a pixel size could result in a significantly\n"
        "oversampled image.  Current pixel size is %g, you requested %g.\n",
	sarRes, pixel_size);
  }
    
  asfPrintStatus("SAR Image is %dx%d LxS, %gm pixels.\n",
		 metaSAR->general->line_count, metaSAR->general->sample_count,
		 sarRes);
  asfPrintStatus("DEM Image is %dx%d LxS, %gm pixels.\n",
		 metaDEM->general->line_count, metaDEM->general->sample_count,
		 demRes);
  if (userMaskFile)
  {
  	asfPrintStatus("User Mask Image is %dx%d LxS, %gm pixels.\n",
                       metamask->general->line_count,
                       metamask->general->sample_count, maskRes);
  }

  // Downsample the SAR image closer to the reference DEM if needed.
  // Otherwise, the quality of the resulting terrain corrected SAR image 
  // suffers. We put in a threshold of 1.5 times the resolution of the SAR
  // image. The -no-resample option overrides this default behavior.
  if (do_resample && 
      (force_resample || demRes > 1.5 * sarRes || pixel_size > 0)) 
  {
    if (pixel_size <= 0)
    {
      asfPrintStatus("DEM resolution is significantly lower than "
                     "SAR resolution.\n");
      pixel_size = demRes;
      asfPrintStatus("Resampling (Downsampling) SAR image to pixel size "
                     "of %g meter%s.\n", pixel_size,
                     pixel_size==1 ? "" : "s");
    }
    else
    {
        asfPrintStatus("Resampling (%s) SAR image to requested pixel size "
                       "of %g meter%s.\n",
                       pixel_size > sarRes ? "Downsampling" : "Oversampling",
                       pixel_size, pixel_size==1 ? "" : "s");
    }
    resampleFile = outputName(output_dir, sarFile, "_resample");

    // In slant range, must scale based on azimuth.
    // In ground range, can resample directly to the proper pixel size
    if (metaSAR->sar->image_type == 'S') {
        double scalfact;
        scalfact = metaSAR->general->y_pixel_size/pixel_size;
        resample(sarFile, resampleFile, scalfact, scalfact);
    } else {
        resample_to_square_pixsiz(sarFile, resampleFile, pixel_size);
    }

    meta_free(metaSAR);
    metaSAR = meta_read(resampleFile);

    asfPrintStatus("After resampling, SAR Image is %dx%d LxS, %gm pixels.\n",
		   metaSAR->general->line_count,
		   metaSAR->general->sample_count,
		   metaSAR->general->x_pixel_size);
  } else {
    pixel_size = sarRes;
    resampleFile = strdup(sarFile);
  }

  // Calculate the slant range pixel size to pass into the ground range to
  // slant range conversion. This ensures that we have square pixels in the
  // output and don't need to scale the image afterwards.
  if (metaSAR->sar->image_type != 'S') {
    srFile = appendSuffix(sarFile, "_slant");
    double sr_pixel_size = 
      (meta_get_slant(metaSAR,0,metaSAR->general->sample_count) -
       meta_get_slant(metaSAR,0,0)) / metaSAR->general->sample_count;
    asfPrintStatus("Converting to Slant Range...\n");

    gr2sr_pixsiz_pp(resampleFile, srFile, sr_pixel_size);

    meta_free(metaSAR);
    metaSAR = meta_read(srFile);

    asfPrintStatus("In slant range, SAR Image is %dx%d LxS, %gm pixels.\n",
		   metaSAR->general->line_count,
		   metaSAR->general->sample_count,
		   metaSAR->general->x_pixel_size);
  } else {
    srFile = strdup(resampleFile);
  }

  lsMaskFile = appendToBasename(outFile, "_mask");

  // Assign a couple of file names and match the DEM
  demTrimSimAmp = outputName(output_dir, demFile, "_sim_amp_trim");
  demTrimSlant = outputName(output_dir, demFile, "_slant_trim");
  match_dem(metaSAR, sarFile, demFile, srFile, output_dir, userMaskFile,
            demTrimSimAmp, demTrimSlant, userMaskClipped, dem_grid_size,
            do_corner_matching, do_fftMatch_verification,
            FALSE, TRUE, TRUE, madssap, clean_files, &t_offset, &x_offset);

  if (do_terrain_correction)
  {            
      // Terrain correct the slant range image while bringing it back to
      // ground range geometry. This is done without radiometric correction
      // of the values.
      ensure_ext(&demTrimSlant, "img");
      ensure_ext(&srFile, "img");
      asfPrintStatus("Terrain correcting slant range image...\n");
      padFile = outputName(output_dir, srFile, "_pad");
      deskewDemFile = outputName(output_dir, srFile, "_dd");
      deskewDemMask = outputName(output_dir, srFile, "_ddm");
      trim(srFile, padFile, 0, 0, metaSAR->general->sample_count + PAD,
           metaSAR->general->line_count);
      deskew_dem(demTrimSlant, deskewDemFile, padFile, 0, userMaskClipped,
		 deskewDemMask, do_interp, maskfill);
      
      // After deskew_dem, there will likely be zeros on the left & right edges
      // of the image, we trim those off before finishing up.
      int startx, endx;
      trim_zeros(deskewDemFile, outFile, &startx, &endx);
      trim(deskewDemMask, lsMaskFile, startx, 0, endx,
           metaSAR->general->line_count);
      //clean(padFile);
      //clean(deskewDemFile);
      //clean(deskewDemMask);

      // Because of the PP earth radius sr->gr fix, we may not have ended
      // up with the same x pixel size that the user requested.  So we will
      // just resample to the size that was requested.
      meta_free(metaSAR);
      metaSAR = meta_read(outFile);
      if (fabs(metaSAR->general->x_pixel_size - pixel_size) > 0.01) {
          asfPrintStatus("Resampling to proper range pixel size...\n");
          resampleFile_2 = outputName(output_dir, outFile, "_resample");
          renameImgAndMeta(outFile, resampleFile_2);
          resample_to_square_pixsiz(resampleFile_2, outFile, pixel_size);
          char *lsMaskFile_2 = 
              outputName(output_dir, lsMaskFile, "_resample");
          renameImgAndMeta(lsMaskFile, lsMaskFile_2);
          resample_to_square_pixsiz(lsMaskFile_2, lsMaskFile, pixel_size);
          //clean(lsMaskFile_2);
      } else {
          resampleFile_2 = NULL;
      }
  }
  else
  {
      // Just need to update the original metadata
      meta_free(metaSAR);

      metaSAR = meta_read(sarFile);
      metaSAR->sar->time_shift += t_offset;
      metaSAR->sar->slant_shift += x_offset;
      meta_write(metaSAR, outFile);
  }

  if (clean_files) {
    asfPrintStatus("Removing intermediate files...\n");
    clean(demTrimSlant);
    clean(demTrimSimAmp);
    clean(resampleFile);
    clean(srFile);
    clean(resampleFile_2);
  }

  asfPrintStatus("Done!\n");

  FREE(resampleFile);
  FREE(srFile);
  FREE(padFile);
  FREE(deskewDemFile);
  FREE(deskewDemMask);
  FREE(lsMaskFile);
  FREE(resampleFile_2);
  FREE(userMaskClipped);

  meta_free(metaSAR);
  meta_free(metaDEM);

  free(output_dir);

  return FALSE;
}
