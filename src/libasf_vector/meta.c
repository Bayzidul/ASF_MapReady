#include "asf.h"
#include "shapefil.h"
#include "asf_vector.h"

// Some of the functionality here is used to convert CEOS leader files as well
// as metadata into the various. When comparing the results from the two 
// sources you will find a few differences.
//
// The following parameters can vary depending on the ingest parameters:
// data_type
// image_data_type
// bands
// earth_radius_pp
//
// All these are not deal with in particular within 'meta_init_ceos' but in
// 'import_ceos' instead.

void shape_meta_init(char *inFile, meta_parameters *meta)
{
  char *dbaseFile;
  DBFHandle dbase;
  SHPHandle shape;
  dbf_header_t *dbf;
  int ii, nCols, length;

  // Read configuration file
  read_header_config("META", &dbf, &nCols);

  // Open database for initialization
  dbaseFile = (char *) MALLOC(sizeof(char)*(strlen(inFile)+5));
  sprintf(dbaseFile, "%s.dbf", inFile);
  dbase = DBFCreate(dbaseFile);
  if (!dbase)
    asfPrintError("Could not create database file '%s'\n", dbaseFile);

  // Add fields to database 
  for (ii=0; ii<nCols; ii++) {
    if (strcmp(dbf[ii].header, "meta.general.basename") == 0 &&
	dbf[ii].visible) {
      length = strlen(meta->general->basename);
      if (DBFAddField(dbase, "Basename", FTString, length, 0) == -1)
	asfPrintError("Could not add basename field to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.sensor") == 0 && 
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Sensor", FTString, 15, 0) == -1)
	asfPrintError("Could not add sensor field to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.sensor_name") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Name", FTString, 15, 0) == -1)
	asfPrintError("Could not add sensor name to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.mode") == 0 && 
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Mode", FTString, 15, 0) == -1)
        asfPrintError("Could not add mode to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.processor") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Processor", FTString, 25, 0) == -1)
        asfPrintError("Could not add processor to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.data_type") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Data_type", FTString, 25, 0) == -1)
        asfPrintError("Could not add data type to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.image_data_type") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Img_data_t", FTString, 25, 0) == -1)
        asfPrintError("Could not add image data type to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.radiometry") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Radiometry", FTString, 15, 0) == -1)
        asfPrintError("Could not add radiometry to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.system") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "System", FTString, 15, 0) == -1)
        asfPrintError("Could not add system to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.acquisition_date") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Acq_date", FTString, 25, 0) == -1)
        asfPrintError("Could not add acquisition date to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.orbit") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Orbit", FTInteger, 7, 0) == -1)
        asfPrintError("Could not add orbit to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.orbit_direction") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Direction", FTString, 20, 0) == -1)
        asfPrintError("Could not add orbit direction to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.frame") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Frame", FTInteger, 5, 0) == -1)
        asfPrintError("Could not add processor to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.band_count") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Band_count", FTInteger, 3, 0) == -1)
        asfPrintError("Could not add band count to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.bands") == 0 &&
	     dbf[ii].visible) {
      length = strlen(meta->general->bands) + 1;
      if (DBFAddField(dbase, "Bands", FTString, length, 0) == -1)
        asfPrintError("Could not add bands to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.line_count") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Lines", FTInteger, 7, 0) == -1)
        asfPrintError("Could not add line count to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.sample_count") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Samples", FTInteger, 7, 0) == -1)
        asfPrintError("Could not add sample count to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.start_line") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Start_line", FTInteger, 7, 0) == -1)
        asfPrintError("Could not add start line to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.start_sample") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Start_sample", FTInteger, 7, 0) == -1)
        asfPrintError("Could not add start sample to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.x_pixel_size") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "X_pix_size", FTDouble, 16, 7) == -1)
        asfPrintError("Could not add x pixel size to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.y_pixel_size") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Y_pix_size", FTDouble, 16, 7) == -1)
        asfPrintError("Could not add y pixel size to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.center_latitude") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Center_lat", FTDouble, 9, 4) == -1)
        asfPrintError("Could not add center latitude to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.center_longitude") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Center_lon", FTDouble, 9, 4) == -1)
        asfPrintError("Could not add center longitude to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.re_major") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "RE_major", FTDouble, 16, 7) == -1)
        asfPrintError("Could not add RE major to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.re_minor") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "RE major", FTDouble, 16, 7) == -1)
        asfPrintError("Could not add RE minor to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.bit_error_rate") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "BER", FTDouble, 16, 7) == -1)
        asfPrintError("Could not add bit error rate to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.missing_lines") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "Miss_lines", FTInteger, 7, 0) == -1)
        asfPrintError("Could not add missing lines to database file\n");
    }
    else if (strcmp(dbf[ii].header, "meta.general.no_data") == 0 &&
	     dbf[ii].visible) {
      if (DBFAddField(dbase, "No_data", FTDouble, 16, 7) == -1)
        asfPrintError("Could not add no data to database file\n");
    }
    if (meta->sar) {
      if (strcmp(dbf[ii].header, "meta.sar.image_type") == 0 && 
	  dbf[ii].visible) {
	if (DBFAddField(dbase, "Image_type", FTString, 25, 0) == -1)
	  asfPrintError("Could not add image type to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.look_direction") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Look_dir", FTString, 25, 0) == -1)
          asfPrintError("Could not add look direction to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.look_count") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Looks", FTInteger, 3, 0) == -1)
          asfPrintError("Could not add look count field to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.deskewed") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Deskewed", FTInteger, 1, 0) == -1)
          asfPrintError("Could not add deskewed field to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.original_line_count") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Org_lines", FTInteger, 7, 0) == -1)
          asfPrintError("Could not add original lines to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.original_sample_count") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Org_samples", FTInteger, 7, 0) == -1)
          asfPrintError("Could not add original samples to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.line_increment") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Line_inc", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add line increment to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.sample_increment") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Sample_inc", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add sample increment to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.range_time_per_pixel") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Rng_t_pix", FTDouble, 16, 12) == -1)
          asfPrintError("Could not add range time per pix to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.azimuth_time_per_pixel") == 0 
	       && dbf[ii].visible) {
        if (DBFAddField(dbase, "Az_t_pix", FTDouble, 16, 12) == -1)
          asfPrintError("Could not add azimuth time pixel to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.slant_range_first_pixel") == 0
               && dbf[ii].visible) {
        if (DBFAddField(dbase, "Slnt_range", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add slant range 1. pix to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.slant_shift") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Slnt_shift", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add slant shift to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.time_shift") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Time_shift", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add time shift to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.wavelength") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Wavelength", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add wavelength to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.prf") == 0 && 
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "PRF", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add PRF to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Earth_rad", FTDouble, 12, 4) == -1)
          asfPrintError("Could not add earth radius to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius_pp") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "ER_pp", FTDouble, 12, 4) == -1)
          asfPrintError("Could not add earth radius pp to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_height") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Sat_height", FTDouble, 12, 4) == -1)
          asfPrintError("Could not add satellite height to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_binary_time") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Sat_bin_t", FTString, 25, 0) == -1)
          asfPrintError("Could not add sat binary time to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_clock_time") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Sat_clock_t", FTString, 25, 0) == -1)
          asfPrintError("Could not add sat clock time to database file\n");
      }
      else if (strcmp(dbf[ii].header, 
		      "meta.sar.range_doppler_coefficients") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Rng_dop_1", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add range doppler 1 to database file\n");
        if (DBFAddField(dbase, "Rng_dop_2", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add range doppler 2 to database file\n");
        if (DBFAddField(dbase, "Rng_dop_3", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add range doppler 3 to database file\n");
      }
      else if (strcmp(dbf[ii].header, 
		      "meta.sar.azimuth_doppler_coefficients") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Az_dop_1", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add azimuth doppler 1 to database file\n");
        if (DBFAddField(dbase, "Az_dop_2", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add azimuth doppler 2 to database file\n");
        if (DBFAddField(dbase, "Az_dop_3", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add azimuth doppler 3 to database file\n");
      }
      else if (strcmp(dbf[ii].header, 
		      "meta.sar.azimuth_processing_bandwidth") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Az_proc_bw", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add az processing bw to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.chirp_rate") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Chirp_rate", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add chirp rate to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.pulse_duration") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Pulse_dur", FTDouble, 16, 12) == -1)
          asfPrintError("Could not add pulse duration to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.range_sampling_rate") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Rng_samp_r", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add range samp rate to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.polarization") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Polarize", FTString, 25, 0) == -1)
          asfPrintError("Could not add polarization to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.multilook") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Multilook", FTInteger, 1, 0) == -1)
          asfPrintError("Could not add multiook to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.pitch") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Pitch", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add pitch to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.roll") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Roll", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add roll to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.sar.yaw") == 0 && 
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Yaw", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add yaw to database file\n");
      }
    }
    if (meta->optical) {
      if (strcmp(dbf[ii].header, "meta.optical.pointing_direction") == 0 &&
	  dbf[ii].visible) {
        if (DBFAddField(dbase, "Point_dir", FTString, 15, 0) == -1)
          asfPrintError("Could not add pointing direction to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.optical.off_nadir_angle") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Off_nadir", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add off nadir angle to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.optical.correction_level") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Corr_level", FTString, 5, 0) == -1)
          asfPrintError("Could not add correction level to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.optical.cloud_percentage") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Cloud_perc", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add cloud percentage to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.optical.sun_azimuth_angle") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Sun_az_ang", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add sun azimuth angle to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.optical.sun_elevation_angle") == 0
	       && dbf[ii].visible) {
        if (DBFAddField(dbase, "Sun_elev", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add sun elevation to database file\n");
      }
    }
    if (meta->thermal) {
      if (strcmp(dbf[ii].header, "meta.thermal.band_gain") == 0 &&
	  dbf[ii].visible) {
        if (DBFAddField(dbase, "Band_gain", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add band gain to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.thermal.band_gain_change") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Gain_change", FTDouble, 16, 7) == -1)
          asfPrintError("Could not band gain change to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.thermal.day") == 0 &&
	       dbf[ii].visible) {
      }
        if (DBFAddField(dbase, "Day", FTInteger, 1, 0) == -1)
          asfPrintError("Could not band day flag to database file\n");
    }
    if (meta->transform) {
      int kk;
      char header[12];
      int n = meta->transform->parameter_count;
      if (strcmp(dbf[ii].header, "meta.transform.parameter_count") == 0 &&
	  dbf[ii].visible) {
        if (DBFAddField(dbase, "Parameters", FTInteger, 2, 0) == -1)
          asfPrintError("Could not add parameter count to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.transform.x") == 0 &&
	       dbf[ii].visible) {
	for (kk=0; kk<n; kk++) {
	  sprintf(header, "X[%d]", kk+1);
	  if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
	    asfPrintError("Could not add parameter x to database file\n");
	}
      }
      else if (strcmp(dbf[ii].header, "meta.transform.y") == 0 &&
	       dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(header, "Y[%d]", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add parameter y to database file\n");
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.l") == 0 &&
	       dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(header, "L[%d]", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add parameter l to database file\n");
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.s") == 0 &&
	       dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(header, "S[%d]", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add parameter s to database file\n");
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.incid_a") == 0 &&
	       dbf[ii].visible) {
        for (kk=0; kk<6; kk++) {
          sprintf(header, "Incid_a[%d]", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add incidence angle to database file\n");
        }
      }
    }
    if (meta->airsar) {
      if (strcmp(dbf[ii].header, "meta.airsar.scale_factor") == 0 &&
	  dbf[ii].visible) {
        if (DBFAddField(dbase, "Scale", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add scale factor to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.gps_altitude") == 0 &&
	  dbf[ii].visible) {
        if (DBFAddField(dbase, "GPS_height", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add GPS altitude to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.lat_peg_point") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Lat_peg_pt", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add lat peg point to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.lon_peg_point") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Lon_peg_pt", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add lon peg point to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.head_peg_point") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Head_pegpt", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add heading peg point to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.along_track_offset") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "AT_offset", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add at offset  to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.cross_track_offset") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "CT_offset", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add ct offset to database file\n");
      }
    }
    if (meta->projection) {
      if (strcmp(dbf[ii].header, "meta.projection.type") == 0 && 
	  dbf[ii].visible) {
        if (DBFAddField(dbase, "Proc_type", FTString, 16, 7) == -1)
          asfPrintError("Could not add projection type to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.startX") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Start_x", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add start x to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.startY") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Start_y", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add start y to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.perX") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Per_x", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add per x to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.perY") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Per_y", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add per y to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.units") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Units", FTString, 12, 0) == -1)
          asfPrintError("Could not add units to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.hem") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Hemisphere", FTString, 10, 0) == -1)
          asfPrintError("Could not add hemisphere to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.spheroid") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Spheroid", FTString, 16, 0) == -1)
          asfPrintError("Could not add speroid to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.re_major") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Proc_major", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add re major to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.re_minor") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Proc_minor", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add re minor to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.datum") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Datum", FTDouble, 35, 0) == -1)
          asfPrintError("Could not add datum to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.projection.height") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Proc_ht", FTDouble, 16, 7) == -1)
          asfPrintError("Could not projection height to database file\n");
      }
      if (meta->projection->type == ALBERS_EQUAL_AREA) {
	if (strcmp(dbf[ii].header, 
		   "meta.projection.param.albers.std_parallel1") == 0 &&
	    dbf[ii].visible) {
	  if (DBFAddField(dbase, "Std_par_1", FTDouble, 9, 4) == -1)
	    asfPrintError("Could not add std parallel 1 to database file\n");
	}
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.albers.std_parallel2") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Std_par_2", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add std parallel 2 to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.albers.center_meridian") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Center_mer", FTDouble, 9, 4) == -1)
            asfPrintError("Could not central meridian to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.albers.orig_latitude") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Orig_lat", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add original lat to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.albers.false_easting") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "False_east", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false easting to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.albers.false_northing") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Flse_north", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add std parallel 1 to database file\n");
        }
      }
      else if (meta->projection->type == SCANSAR_PROJECTION) {
	if (strcmp(dbf[ii].header, "meta.projection.param.atct.rlocal") == 0 &&
	    dbf[ii].visible) {
          if (DBFAddField(dbase, "R_local", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add rlocal to database file\n");
        }
	else if (strcmp(dbf[ii].header, 
			"meta.projection.param.atct.alpha1") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Alpha_1", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add alpha1 to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.atct.alpha2") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Alpha_2", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add alpha 2 to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.atct.alpha3") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Alphar_3", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add alpha 3 to database file\n");
        }
      }
      else if (meta->projection->type == LAMBERT_AZIMUTHAL_EQUAL_AREA) {
	if (strcmp(dbf[ii].header, 
		   "meta.projection.param.lamaz.center_lat") == 0 &&
	    dbf[ii].visible) {
          if (DBFAddField(dbase, "Center_lat", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add center latitude to database file\n");
        }
	else if (strcmp(dbf[ii].header, 
			"meta.projection.param.lamaz.center_lon") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Center_lon", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add center longitude to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.lamaz.false_easting") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "False_east", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false easting to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.lamaz.false_northing") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Flse_north", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false northing to database file\n");
        }
      }
      else if (meta->projection->type == LAMBERT_CONFORMAL_CONIC) {
	if (strcmp(dbf[ii].header, "meta.projection.param.lamcc.plat1") == 0 &&
	    dbf[ii].visible) {
          if (DBFAddField(dbase, "Std_par_1", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add std parallel 1 to database file\n");
        }
	else if (strcmp(dbf[ii].header, 
			"meta.projection.param.lamcc.plat2") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Std_par_2", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add std parallel 1 to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.lamcc.lat0") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Orig_lat", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add lat of origin to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.lamcc.lon0") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Center_mer", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add central meridian to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.lamcc.false_easting") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "False_east", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false easting to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.lamcc.false_northing") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Flse_north", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false northing to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.lamcc.scale_factor") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Proc_scale", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add scale factor to database file\n");
        }
      }
      else if (meta->projection->type == POLAR_STEREOGRAPHIC) {
	if (strcmp(dbf[ii].header, "meta.projection.param.ps.slat") == 0 &&
	    dbf[ii].visible) {
          if (DBFAddField(dbase, "Std_par", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add std parallel to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.projection.param.ps.slon") == 0 
		 && dbf[ii].visible) {
          if (DBFAddField(dbase, "Center_mer", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add central meridian to database file\n");
        }
	else if (strcmp(dbf[ii].header, 
			"meta.projection.param.ps.false_easting") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "False_east", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false easting to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.ps.false_northing") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Flse_north", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false northing to database file\n");
        }
      }
      else if (meta->projection->type == UNIVERSAL_TRANSVERSE_MERCATOR) {
	if (strcmp(dbf[ii].header, "meta.projection.param.utm.zone") == 0 &&
	    dbf[ii].visible) {
          if (DBFAddField(dbase, "Zone", FTDouble, 2, 0) == -1)
            asfPrintError("Could not add zone to database file\n");
        }
	else if (strcmp(dbf[ii].header, 
			"meta.projection.param.utm.false_easting") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "False_east", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false easting to database file\n");
        }
	else if (strcmp(dbf[ii].header,
			"meta.projection.param.utm.false_northing") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Flse_north", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add false northing to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.projection.param.utm.lat0") == 0
		 && dbf[ii].visible) {
          if (DBFAddField(dbase, "Proc_lat", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add latitude to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.projection.param.utm.lon0") == 0
		 && dbf[ii].visible) {
          if (DBFAddField(dbase, "Proc_lon", FTDouble, 9, 4) == -1)
            asfPrintError("Could not add std parallel 1 to database file\n");
        }
	else if (strcmp(dbf[ii].header, 
			"meta.projection.param.utm.scale_factor") == 0 &&
		 dbf[ii].visible) {
          if (DBFAddField(dbase, "Proc_scale", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add scale factor to database file\n");
        }
      }
      else if (meta->projection->type == STATE_PLANE) {
	if (strcmp(dbf[ii].header, "meta.projection.param.state.zone") == 0 &&
	    dbf[ii].visible) {
          if (DBFAddField(dbase, "Zone", FTDouble, 16, 7) == -1)
            asfPrintError("Could not add std parallel 1 to database file\n");
        }
      }
    }
    if (meta->stats) {
      int kk;
      int n = meta->stats->band_count;
      char header[12];
      if (strcmp(dbf[ii].header, "meta.stats.band_count") == 0 &&
	  dbf[ii].visible) {
	if (DBFAddField(dbase, "Stat_bands", FTDouble, 16, 7) == -1)
	  asfPrintError("Could not add band count to database file\n");
      }
      for (kk=0; kk<n; kk++) {
	if (strcmp(dbf[ii].header, "meta.stats.band_stats.band_id") == 0 &&
	    dbf[ii].visible) {
	  sprintf(header, "Band_ID_%d", kk+1);
	  length = strlen(meta->stats->band_stats[kk].band_id);
          if (DBFAddField(dbase, header, FTString, length, 0) == -1)
            asfPrintError("Could not add band ID to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.stats.band_stats.min") == 0 &&
		 dbf[ii].visible) {
	  sprintf(header, "Minimum_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add minimum to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.stats.band_stats.max") == 0 &&
		 dbf[ii].visible) {
	  sprintf(header, "Maximum_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add maximum to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.stats.band_stats.mean") == 0 &&
		 dbf[ii].visible) {
	  sprintf(header, "Mean_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add mean to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.stats.band_stats.rmse") == 0 &&
		 dbf[ii].visible) {
	  sprintf(header, "RMSE_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add root mean square to database file\n");
        }
	else if (strcmp(dbf[ii].header, 
			"meta.stats.band_stats.std_deviation") == 0 &&
		 dbf[ii].visible) {
	  sprintf(header, "Std_dev_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add std deviation to database file\n");
        }
	else if (strcmp(dbf[ii].header, "meta.stats.band_stats.mask") == 0 &&
		 dbf[ii].visible) {
	  sprintf(header, "Mask_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add mask to database file\n");
        }
      }
    }
    if (meta->state_vectors) {
      int kk;
      int n = meta->state_vectors->vector_count;
      char header[12];
      if (strcmp(dbf[ii].header, "meta.state.year") == 0 && dbf[ii].visible) {
	if (DBFAddField(dbase, "Year", FTInteger, 4, 0) == -1)
	  asfPrintError("Could not add vector count to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.state.julDay") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Julian_day", FTInteger, 3, 0) == -1)
          asfPrintError("Could not add julian day to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.state.second") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Second", FTDouble, 16, 7) == -1)
          asfPrintError("Could not add second to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.state.vector_count") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Num_stVec", FTInteger, 2, 0) == -1)
          asfPrintError("Could not add std parallel 1 to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.state.vectors") == 0 &&
	       dbf[ii].visible) {
	for (kk=0; kk<n; kk++) {
	  sprintf(header, "Time_%d", kk+1);
	  if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
	    asfPrintError("Could not add stVec time to database file\n");
          sprintf(header, "Pos_x_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add stVec time to database file\n");
          sprintf(header, "Pos_y_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add stVec time to database file\n");
          sprintf(header, "Pos_z_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add stVec time to database file\n");
          sprintf(header, "Vel_x_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add stVec time to database file\n");
          sprintf(header, "Vel_y_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add stVec time to database file\n");
          sprintf(header, "Vel_z_%d", kk+1);
          if (DBFAddField(dbase, header, FTDouble, 16, 7) == -1)
            asfPrintError("Could not add stVec time to database file\n");
	}
      }
    }
    if (meta->location) {
      if (strcmp(dbf[ii].header, "meta.location.lat_start_near_range") == 0 &&
	  dbf[ii].visible) {
        if (DBFAddField(dbase, "Lat_1", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lat start near rng to database file\n");
      }
      else if (strcmp(dbf[ii].header, 
		      "meta.location.lon_start_near_range") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Lon_1", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lon start near rng to database file\n");
      }
      else if (strcmp(dbf[ii].header,
		      "meta.location.lat_start_far_range") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Lat_2", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lat start far rng to database file\n");
      }
      else if (strcmp(dbf[ii].header,
		      "meta.location.lon_start_far_range") == 0 &&
	       dbf[ii].visible) {
        if (DBFAddField(dbase, "Lon_2", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lon start far rng to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.location.lat_end_near_range") == 0
	       && dbf[ii].visible) {
        if (DBFAddField(dbase, "Lat_3", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lat end near rng to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.location.lon_end_near_range") == 0
	       && dbf[ii].visible) {
        if (DBFAddField(dbase, "Lon_3", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lon end near rng to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.location.lat_end_far_range") == 0
	       && dbf[ii].visible) {
        if (DBFAddField(dbase, "Lat_4", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lat end far rng to database file\n");
      }
      else if (strcmp(dbf[ii].header, "meta.location.lon_end_far_range") == 0
	       && dbf[ii].visible) {
        if (DBFAddField(dbase, "Lon_4", FTDouble, 9, 4) == -1)
          asfPrintError("Could not add lon end far rng to database file\n");
      }
    }
  }    

  // Close the database for initialization
  DBFClose(dbase);

  // Open shapefile for initialization
  shape = SHPCreate(inFile, SHPT_POLYGON);
  if (!shape)
    asfPrintError("Could not create shapefile '%s'\n", inFile);

  // Close shapefile for initialization
  SHPClose(shape);

  FREE(dbaseFile);

  return;
}

// Convert metadata to generic csv file
int meta2csv(char *inFile, char *outFile, int listFlag)
{
  FILE *fp;
  meta_parameters *meta;
  dbf_header_t *dbf;
  int ii, nCols;
  char *header = (char *) MALLOC(sizeof(char)*4096);
  char *line = (char *) MALLOC(sizeof(char)*4096);

  // Read configuration file
  read_header_config("META", &dbf, &nCols);

  // Read metadata file
  meta = meta_read(inFile);

  // Open output file
  fp = FOPEN(outFile, "w");
  strcpy(header, "");
  
  // Write header line
  for (ii=0; ii<nCols; ii++) {
    // General block
    if (strcmp(dbf[ii].header, "meta.general.basename") == 0 &&
        dbf[ii].visible) 
      strcat(header, "Basename,");
    else if (strcmp(dbf[ii].header, "meta.general.sensor") == 0 &&
             dbf[ii].visible)
      strcat(header, "Sensor,");
    else if (strcmp(dbf[ii].header, "meta.general.sensor_name") == 0 &&
             dbf[ii].visible)
      strcat(header, "Name,");
    else if (strcmp(dbf[ii].header, "meta.general.mode") == 0 &&
             dbf[ii].visible) 
      strcat(header, "Mode,");
    else if (strcmp(dbf[ii].header, "meta.general.processor") == 0 &&
             dbf[ii].visible)
      strcat(header, "Processor,");
    else if (strcmp(dbf[ii].header, "meta.general.data_type") == 0 &&
             dbf[ii].visible)
      strcat(header, "Data_type,");
    else if (strcmp(dbf[ii].header, "meta.general.image_data_type") == 0 &&
             dbf[ii].visible)
      strcat(header, "Img_data_t,");
    else if (strcmp(dbf[ii].header, "meta.general.radiometry") == 0 &&
             dbf[ii].visible)
      strcat(header, "Radiometry,");
    else if (strcmp(dbf[ii].header, "meta.general.system") == 0 &&
             dbf[ii].visible)
      strcat(header, "System,");
    else if (strcmp(dbf[ii].header, "meta.general.acquisition_date") == 0 &&
             dbf[ii].visible)
      strcat(header, "Acq_date,");
    else if (strcmp(dbf[ii].header, "meta.general.orbit") == 0 &&
             dbf[ii].visible)
      strcat(header, "Orbit,");
    else if (strcmp(dbf[ii].header, "meta.general.orbit_direction") == 0 &&
             dbf[ii].visible)
      strcat(header, "Direction,");
    else if (strcmp(dbf[ii].header, "meta.general.frame") == 0 &&
             dbf[ii].visible)
      strcat(header, "Frame,");
    else if (strcmp(dbf[ii].header, "meta.general.band_count") == 0 &&
             dbf[ii].visible)
      strcat(header, "Band_count,");
    else if (strcmp(dbf[ii].header, "meta.general.bands") == 0 &&
             dbf[ii].visible)
      strcat(header, "Bands,");
    else if (strcmp(dbf[ii].header, "meta.general.line_count") == 0 &&
             dbf[ii].visible)
      strcat(header, "Lines,");
    else if (strcmp(dbf[ii].header, "meta.general.sample_count") == 0 &&
             dbf[ii].visible)
      strcat(header, "Samples,");
    else if (strcmp(dbf[ii].header, "meta.general.start_line") == 0 &&
             dbf[ii].visible)
      strcat(header, "Start_line,");
    else if (strcmp(dbf[ii].header, "meta.general.start_sample") == 0 &&
             dbf[ii].visible)
      strcat(header, "Start_sample,");
    else if (strcmp(dbf[ii].header, "meta.general.x_pixel_size") == 0 &&
             dbf[ii].visible)
      strcat(header, "X_pix_size,");
    else if (strcmp(dbf[ii].header, "meta.general.y_pixel_size") == 0 &&
             dbf[ii].visible)
      strcat(header, "Y_pix_size,");
    else if (strcmp(dbf[ii].header, "meta.general.center_latitude") == 0 &&
             dbf[ii].visible)
      strcat(header, "Center_lat,");
    else if (strcmp(dbf[ii].header, "meta.general.center_longitude") == 0 &&
             dbf[ii].visible)
      strcat(header, "Center_lon,");
    else if (strcmp(dbf[ii].header, "meta.general.re_major") == 0 &&
             dbf[ii].visible)
      strcat(header, "RE_major,");
    else if (strcmp(dbf[ii].header, "meta.general.re_minor") == 0 &&
             dbf[ii].visible)
      strcat(header, "RE major,");
    else if (strcmp(dbf[ii].header, "meta.general.bit_error_rate") == 0 &&
             dbf[ii].visible)
      strcat(header, "BER,");
    else if (strcmp(dbf[ii].header, "meta.general.missing_lines") == 0 &&
             dbf[ii].visible)
      strcat(header, "Miss_lines,");
    else if (strcmp(dbf[ii].header, "meta.general.no_data") == 0 &&
             dbf[ii].visible)
      strcat(header, "No_data,");
    // SAR block
    if (meta->sar) {
      if (strcmp(dbf[ii].header, "meta.sar.image_type") == 0 &&
          dbf[ii].visible)
        strcat(header, "Image_type,");
      else if (strcmp(dbf[ii].header, "meta.sar.look_direction") == 0 &&
               dbf[ii].visible)
        strcat(header, "Look_dir,");
      else if (strcmp(dbf[ii].header, "meta.sar.look_count") == 0 &&
               dbf[ii].visible)
        strcat(header, "Looks,");
      else if (strcmp(dbf[ii].header, "meta.sar.deskewed") == 0 &&
               dbf[ii].visible)
        strcat(header, "Deskewed,");
      else if (strcmp(dbf[ii].header, "meta.sar.original_line_count") == 0 &&
               dbf[ii].visible)
        strcat(header, "Org_lines,");
      else if (strcmp(dbf[ii].header, "meta.sar.original_sample_count") == 0 &&
               dbf[ii].visible)
        strcat(header, "Org_samples,");
      else if (strcmp(dbf[ii].header, "meta.sar.line_increment") == 0 &&
               dbf[ii].visible)
        strcat(header, "Line_inc,");
      else if (strcmp(dbf[ii].header, "meta.sar.sample_increment") == 0 &&
               dbf[ii].visible)
        strcat(header, "Sample_inc,");
      else if (strcmp(dbf[ii].header, "meta.sar.range_time_per_pixel") == 0 &&
               dbf[ii].visible)
	strcat(header, "Rng_t_pix,");
      else if (strcmp(dbf[ii].header, "meta.sar.azimuth_time_per_pixel") == 0
               && dbf[ii].visible)
        strcat(header, "Az_t_pix,");
      else if (strcmp(dbf[ii].header, "meta.sar.slant_range_first_pixel") == 0
               && dbf[ii].visible)
        strcat(header, "Slant_range,");
      else if (strcmp(dbf[ii].header, "meta.sar.slant_shift") == 0 &&
               dbf[ii].visible)
        strcat(header, "Slant_shift,");
      else if (strcmp(dbf[ii].header, "meta.sar.time_shift") == 0 &&
               dbf[ii].visible)
        strcat(header, "Time_shift,");
      else if (strcmp(dbf[ii].header, "meta.sar.wavelength") == 0 &&
               dbf[ii].visible)
        strcat(header, "Wavelength,");
      else if (strcmp(dbf[ii].header, "meta.sar.prf") == 0 &&
               dbf[ii].visible)
        strcat(header, "PRF,");
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius") == 0 &&
               dbf[ii].visible)
        strcat(header, "Earth_rad,");
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius_pp") == 0 &&
               dbf[ii].visible)
        strcat(header, "ER_pp,");
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_height") == 0 &&
               dbf[ii].visible)
        strcat(header, "Sat_height,");
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_binary_time") == 0 &&
               dbf[ii].visible)
        strcat(header, "Sat_bin_t,");
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_clock_time") == 0 &&
               dbf[ii].visible)
        strcat(header, "Sat_clock_t,");
      else if (strcmp(dbf[ii].header,
                      "meta.sar.range_doppler_coefficients") == 0 &&
               dbf[ii].visible) {
        strcat(header, "Rng_dop_1,");
        strcat(header, "Rng_dop_2,");
        strcat(header, "Rng_dop_3,");
      }
      else if (strcmp(dbf[ii].header,
                      "meta.sar.azimuth_doppler_coefficients") == 0 &&
               dbf[ii].visible) {
        strcat(header, "Az_dop_1,");
        strcat(header, "Az_dop_2,");
        strcat(header, "Az_dop_3,");
      }
      else if (strcmp(dbf[ii].header, 
		      "meta.sar.azimuth_processing_bandwidth") == 0 && 
	       dbf[ii].visible)
	strcat(header, "Az_proc_bw,");
      else if (strcmp(dbf[ii].header, "meta.sar.chirp_rate") == 0 &&
               dbf[ii].visible)
	strcat(header, "Chirp_rate,");
      else if (strcmp(dbf[ii].header, "meta.sar.pulse_duration") == 0 &&
               dbf[ii].visible)
        strcat(header, "Pulse_dur,");
      else if (strcmp(dbf[ii].header, "meta.sar.range_sampling_rate") == 0 &&
               dbf[ii].visible)
        strcat(header, "Rng_samp_r,");
      else if (strcmp(dbf[ii].header, "meta.sar.polarization") == 0 &&
               dbf[ii].visible)
        strcat(header, "Polarization,");
      else if (strcmp(dbf[ii].header, "meta.sar.multilook") == 0 &&
               dbf[ii].visible)
        strcat(header, "Multilook,");
      else if (strcmp(dbf[ii].header, "meta.sar.pitch") == 0 &&
               dbf[ii].visible)
        strcat(header, "Pitch,");
      else if (strcmp(dbf[ii].header, "meta.sar.roll") == 0 &&
               dbf[ii].visible)
        strcat(header, "Roll,");
      else if (strcmp(dbf[ii].header, "meta.sar.yaw") == 0 &&
               dbf[ii].visible)
        strcat(header, "Yaw,");
    }
    // Optical block
    if (meta->optical) {
      if (strcmp(dbf[ii].header, "meta.optical.pointing_direction") == 0 &&
          dbf[ii].visible)
        strcat(header, "Point_dir,");
      else if (strcmp(dbf[ii].header, "meta.optical.off_nadir_angle") == 0 &&
               dbf[ii].visible)
        strcat(header, "Off_nadir,");
      else if (strcmp(dbf[ii].header, "meta.optical.correction_level") == 0 &&
               dbf[ii].visible)
        strcat(header, "Corr_level,");
      else if (strcmp(dbf[ii].header, "meta.optical.cloud_percentage") == 0 &&
               dbf[ii].visible)
        strcat(header, "Cloud_perc,");
      else if (strcmp(dbf[ii].header, "meta.optical.sun_azimuth_angle") == 0 &&
               dbf[ii].visible)
        strcat(header, "Sun_az_ang,");
      else if (strcmp(dbf[ii].header, "meta.optical.sun_elevation_angle") == 0
               && dbf[ii].visible)
        strcat(header, "Sun_elev,");
    }
    // Thermal block
    if (meta->thermal) {
      if (strcmp(dbf[ii].header, "meta.thermal.band_gain") == 0 &&
          dbf[ii].visible)
        strcat(header, "Band_gain,");
      else if (strcmp(dbf[ii].header, "meta.thermal.band_gain_change") == 0 &&
               dbf[ii].visible)
        strcat(header, "Gain_change,");
      else if (strcmp(dbf[ii].header, "meta.thermal.day") == 0 &&
               dbf[ii].visible)
	strcat(header, "Day,");
    }
    // Transform block
    if (meta->transform) {
      int kk;
      char str[12];
      int n = meta->transform->parameter_count;
      if (strcmp(dbf[ii].header, "meta.transform.parameter_count") == 0 &&
          dbf[ii].visible)
        strcat(header, "Parameters,");
      else if (strcmp(dbf[ii].header, "meta.transform.x") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(str, "X[%d],", kk+1);
          strcat(header, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.y") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(str, "Y[%d],", kk+1);
          strcat(header, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.l") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(str, "L[%d],", kk+1);
          strcat(header, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.s") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(str, "S[%d],", kk+1);
          strcat(header, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.incid_a") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<6; kk++) {
          sprintf(str, "Incid_a[%d],", kk+1);
          strcat(header, str);
        }
      }
    }
    // AirSAR block
    if (meta->airsar) {
      if (strcmp(dbf[ii].header, "meta.airsar.scale_factor") == 0 &&
          dbf[ii].visible)
        strcat(header, "Scale,");
      else if (strcmp(dbf[ii].header, "meta.airsar.gps_altitude") == 0 &&
	       dbf[ii].visible)
        strcat(header, "GPS_height,");
      else if (strcmp(dbf[ii].header, "meta.airsar.lat_peg_point") == 0 &&
               dbf[ii].visible)
        strcat(header, "Lat_peg_pt,");
      else if (strcmp(dbf[ii].header, "meta.airsar.lon_peg_point") == 0 &&
               dbf[ii].visible)
        strcat(header, "Lon_peg_pt,");
      else if (strcmp(dbf[ii].header, "meta.airsar.head_peg_point") == 0 &&
               dbf[ii].visible)
        strcat(header, "Head_pegpt,");
      else if (strcmp(dbf[ii].header, "meta.airsar.along_track_offset") == 0 &&
               dbf[ii].visible)
        strcat(header, "AT_offset,");
      else if (strcmp(dbf[ii].header, "meta.airsar.cross_track_offset") == 0 &&
               dbf[ii].visible) 
        strcat(header, "CT_offset,");
    }
    // Projection block
    if (meta->projection) {
      if (strcmp(dbf[ii].header, "meta.projection.type") == 0 &&
          dbf[ii].visible)
        strcat(header, "Proc_type,");
      else if (strcmp(dbf[ii].header, "meta.projection.startX") == 0 &&
               dbf[ii].visible)
        strcat(header, "Start_x,");
      else if (strcmp(dbf[ii].header, "meta.projection.startY") == 0 &&
               dbf[ii].visible)
        strcat(header, "Start_y,");
      else if (strcmp(dbf[ii].header, "meta.projection.perX") == 0 &&
               dbf[ii].visible)
        strcat(header, "Per_x,");
      else if (strcmp(dbf[ii].header, "meta.projection.perY") == 0 &&
               dbf[ii].visible)
        strcat(header, "Per_y,");
      else if (strcmp(dbf[ii].header, "meta.projection.units") == 0 &&
               dbf[ii].visible)
        strcat(header, "Units,");
      else if (strcmp(dbf[ii].header, "meta.projection.hem") == 0 &&
               dbf[ii].visible)
        strcat(header, "Hemisphere,");
      else if (strcmp(dbf[ii].header, "meta.projection.spheroid") == 0 &&
               dbf[ii].visible)
        strcat(header, "Spheroid,");
      else if (strcmp(dbf[ii].header, "meta.projection.re_major") == 0 &&
               dbf[ii].visible)
        strcat(header, "Proc_major,");
      else if (strcmp(dbf[ii].header, "meta.projection.re_minor") == 0 &&
               dbf[ii].visible)
        strcat(header, "Proc_minor,");
      else if (strcmp(dbf[ii].header, "meta.projection.datum") == 0 &&
               dbf[ii].visible)
        strcat(header, "Datum,");
      else if (strcmp(dbf[ii].header, "meta.projection.height") == 0 &&
               dbf[ii].visible)
        strcat(header, "Proc_ht,");
      if (meta->projection->type == ALBERS_EQUAL_AREA) {
        if (strcmp(dbf[ii].header,
                   "meta.projection.param.albers.std_parallel1") == 0 &&
            dbf[ii].visible)
          strcat(header, "Std_par_1,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.std_parallel2") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Std_par_2,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.center_meridian") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Center_mer,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.orig_latitude") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Orig_lat,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.false_easting") == 0 &&
                 dbf[ii].visible)
          strcat(header, "False_east,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.false_northing") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Flse_north,");
      }
      else if (meta->projection->type == SCANSAR_PROJECTION) {
        if (strcmp(dbf[ii].header, "meta.projection.param.atct.rlocal") == 0 &&
            dbf[ii].visible)
          strcat(header, "R_local,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha1") == 0 &&
                 dbf[ii].visible) 
          strcat(header, "Alpha_1,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha2") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Alpha_2,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha3") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Alphar_3,");
      }
      else if (meta->projection->type == LAMBERT_AZIMUTHAL_EQUAL_AREA) {
        if (strcmp(dbf[ii].header,
                   "meta.projection.param.lamaz.center_lat") == 0 &&
            dbf[ii].visible)
          strcat(header, "Center_lat,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.center_lon") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Center_lon,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.false_easting") == 0 &&
                 dbf[ii].visible)
          strcat(header, "False_east,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.false_northing") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Flse_north,");
      }
      else if (meta->projection->type == LAMBERT_CONFORMAL_CONIC) {
        if (strcmp(dbf[ii].header, "meta.projection.param.lamcc.plat1") == 0 &&
            dbf[ii].visible)
          strcat(header, "Std_par_1,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.plat2") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Std_par_2,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.lat0") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Orig_lat,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.lon0") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Center_mer,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.false_easting") == 0 &&
                 dbf[ii].visible)
          strcat(header, "False_east,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.false_northing") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Flse_north,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.scale_factor") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Proc_scale,");
      }
      else if (meta->projection->type == POLAR_STEREOGRAPHIC) {
        if (strcmp(dbf[ii].header, "meta.projection.param.ps.slat") == 0 &&
            dbf[ii].visible)
          strcat(header, "Std_par,");
        else if (strcmp(dbf[ii].header, "meta.projection.param.ps.slon") == 0
                 && dbf[ii].visible)
          strcat(header, "Center_mer,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.false_easting") == 0 &&
                 dbf[ii].visible)
          strcat(header, "False_east,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.false_northing") == 0 &&
                 dbf[ii].visible)
          strcat(header, "False_north,");
      }
      else if (meta->projection->type == UNIVERSAL_TRANSVERSE_MERCATOR) {
        if (strcmp(dbf[ii].header, "meta.projection.param.utm.zone") == 0 &&
            dbf[ii].visible)
          strcat(header, "Zone,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.false_easting") == 0 &&
                 dbf[ii].visible)
          strcat(header, "False_east,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.false_northing") == 0 &&
                 dbf[ii].visible)
          strcat(header, "False_north,");
        else if (strcmp(dbf[ii].header, "meta.projection.param.utm.lat0") == 0
                 && dbf[ii].visible)
          strcat(header, "Proc_lat,");
        else if (strcmp(dbf[ii].header, "meta.projection.param.utm.lon0") == 0
                 && dbf[ii].visible)
          strcat(header, "Proc_lon,");
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.scale_factor") == 0 &&
                 dbf[ii].visible)
          strcat(header, "Proc_scale,");
      }
      else if (meta->projection->type == STATE_PLANE) {
        if (strcmp(dbf[ii].header, "meta.projection.param.state.zone") == 0 &&
            dbf[ii].visible)
          strcat(header, "Zone,");
      }
    }
    // Statistics block
    if (meta->stats) {
      int kk;
      int n = meta->stats->band_count;
      char str[12];
      if (strcmp(dbf[ii].header, "meta.stats.band_count") == 0 &&
          dbf[ii].visible)
        strcat(header, "Stat_bands,");
      for (kk=0; kk<n; kk++) {
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.band_id") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "Band_ID_%d,", kk+1);
          strcat(header, str);
        }
        else if (strcmp(dbf[ii].header, "meta.stats.band_stats.min") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "Minimum_%d,", kk+1);
          strcat(header, str);
        }
        else if (strcmp(dbf[ii].header, "meta.stats.band_stats.max") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "Maximum_%d,", kk+1);
          strcat(header, str);
        }
        else if (strcmp(dbf[ii].header, "meta.stats.band_stats.mean") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "Mean_%d,", kk+1);
          strcat(header, str);
        }
        else if (strcmp(dbf[ii].header, "meta.stats.band_stats.rmse") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "RMSE_%d,", kk+1);
          strcat(header, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.stats.band_stats.std_deviation") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "Std_dev_%d", kk+1);
          strcat(header, str);
        }
        else if (strcmp(dbf[ii].header, "meta.stats.band_stats.mask") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "Mask_%d,", kk+1);
          strcat(header, str);
        }
      }
    }
    // State vector block
    if (meta->state_vectors) {
      int kk;
      int n = meta->state_vectors->vector_count;
      char str[12];
      if (strcmp(dbf[ii].header, "meta.state.year") == 0 && dbf[ii].visible)
        strcat(header, "Year,");
      else if (strcmp(dbf[ii].header, "meta.state.julDay") == 0 &&
               dbf[ii].visible)
        strcat(header, "Julian_day,");
      else if (strcmp(dbf[ii].header, "meta.state.second") == 0 &&
               dbf[ii].visible)
        strcat(header, "Second,");
      else if (strcmp(dbf[ii].header, "meta.state.vector_count") == 0 &&
               dbf[ii].visible)
        strcat(header, "Num_stVec,");
      else if (strcmp(dbf[ii].header, "meta.state.vectors") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(str, "Time_%d,", kk+1);
          strcat(header, str);
          sprintf(str, "Pos_x_%d,", kk+1);
          strcat(header, str);
          sprintf(str, "Pos_y_%d,", kk+1);
          strcat(header, str);
          sprintf(str, "Pos_z_%d,", kk+1);
          strcat(header, str);
          sprintf(str, "Vel_x_%d,", kk+1);
          strcat(header, str);
          sprintf(str, "Vel_y_%d,", kk+1);
          strcat(header, str);
          sprintf(str, "Vel_z_%d,", kk+1);
          strcat(header, str);
        }
      }
    }
    // Location block
    if (meta->location) {
      if (strcmp(dbf[ii].header, "meta.location.lat_start_near_range") == 0 &&
          dbf[ii].visible)
        strcat(header, "Lat_1,");
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_start_near_range") == 0 &&
               dbf[ii].visible)
        strcat(header, "Lon_1,");
      else if (strcmp(dbf[ii].header,
                      "meta.location.lat_start_far_range") == 0 &&
               dbf[ii].visible)
        strcat(header, "Lat_2,");
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_start_far_range") == 0 &&
               dbf[ii].visible)
        strcat(header, "Lon_2,");
      else if (strcmp(dbf[ii].header, "meta.location.lat_end_near_range") == 0
               && dbf[ii].visible)
        strcat(header, "Lat_3,");
      else if (strcmp(dbf[ii].header, "meta.location.lon_end_near_range") == 0
               && dbf[ii].visible)
        strcat(header, "Lon_3,");
      else if (strcmp(dbf[ii].header, "meta.location.lat_end_far_range") == 0
               && dbf[ii].visible)
        strcat(header, "Lat_4,");
      else if (strcmp(dbf[ii].header, "meta.location.lon_end_far_range") == 0
               && dbf[ii].visible)
        strcat(header, "Lon_4,");
    }
  }
  header[strlen(header)-1] = '\0';
  fprintf(fp, "%s\n", header);

  char str[255];

  // Write line
  for (ii=0; ii<nCols; ii++) {
    // General block
    if (strcmp(dbf[ii].header, "meta.general.basename") == 0 &&
        dbf[ii].visible) {
      sprintf(str, "%s,", meta->general->basename);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.sensor") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", meta->general->sensor);
      strcat(line, str);      
    }
    else if (strcmp(dbf[ii].header, "meta.general.sensor_name") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", meta->general->sensor_name);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.mode") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", meta->general->mode);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.processor") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", meta->general->processor);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.data_type") == 0 &&
             dbf[ii].visible) {
      char data_type[25];
      if (meta->general->data_type == BYTE)
        strcpy(data_type, "BYTE");
      else if (meta->general->data_type == INTEGER16)
        strcpy(data_type, "INTEGER16");
      else if (meta->general->data_type == INTEGER32)
        strcpy(data_type, "INTEGER32");
      else if (meta->general->data_type == REAL32)
        strcpy(data_type, "REAL32");
      else if (meta->general->data_type == REAL64)
        strcpy(data_type, "REAL64");
      else if (meta->general->data_type == COMPLEX_BYTE)
        strcpy(data_type, "COMPLEX_BYTE");
      else if (meta->general->data_type == COMPLEX_INTEGER16)
        strcpy(data_type, "COMPLEX_INTEGER16");
      else if (meta->general->data_type == COMPLEX_INTEGER32)
        strcpy(data_type, "COMPLEX_INTEGER32");
      else if (meta->general->data_type == COMPLEX_REAL32)
        strcpy(data_type, "COMPLEX_REAL32");
      else if (meta->general->data_type == COMPLEX_REAL64)
        strcpy(data_type, "COMPLEX_REAL64");
      sprintf(str, "%s,", data_type);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.image_data_type") == 0 &&
             dbf[ii].visible) {
      char image_data_type[25];
      if (meta->general->image_data_type == RAW_IMAGE)
        strcpy(image_data_type, "RAW_IMAGE");
      else if (meta->general->image_data_type == COMPLEX_IMAGE)
        strcpy(image_data_type, "COMPLEX_IMAGE");
      else if (meta->general->image_data_type == AMPLITUDE_IMAGE)
        strcpy(image_data_type, "AMPLITUDE_IMAGE");
      else if (meta->general->image_data_type == PHASE_IMAGE)
        strcpy(image_data_type, "PHASE_IMAGE");
      else if (meta->general->image_data_type == INTERFEROGRAM)
        strcpy(image_data_type, "INTERFEROGRAM");
      else if (meta->general->image_data_type == COHERENCE_IMAGE)
        strcpy(image_data_type, "COHERENCE_IMAGE");
      else if (meta->general->image_data_type == POLARIMETRIC_IMAGE)
        strcpy(image_data_type, "POLARIMETRIC_IMAGE");
      else if (meta->general->image_data_type == LUT_IMAGE)
        strcpy(image_data_type, "LUT_IMAGE");
      else if (meta->general->image_data_type == ELEVATION)
        strcpy(image_data_type, "ELEVATION");
      else if (meta->general->image_data_type == DEM)
        strcpy(image_data_type, "DEM");
      else if (meta->general->image_data_type == IMAGE)
        strcpy(image_data_type, "IMAGE");
      else if (meta->general->image_data_type == MASK)
        strcpy(image_data_type, "MASK");
      sprintf(str, "%s,", image_data_type);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.radiometry") == 0 &&
             dbf[ii].visible) {
      char radiometry[20];
      if (meta->general->radiometry == r_AMP)
        strcpy(radiometry, "AMPLITUDE");
      else if (meta->general->radiometry == r_SIGMA)
        strcpy(radiometry, "SIGMA");
      else if (meta->general->radiometry == r_BETA)
        strcpy(radiometry, "BETA");
      else if (meta->general->radiometry == r_GAMMA)
        strcpy(radiometry, "GAMMA");
      else if (meta->general->radiometry == r_SIGMA_DB)
        strcpy(radiometry, "SIGMA_DB");
      else if (meta->general->radiometry == r_BETA_DB)
        strcpy(radiometry, "BETA_DB");
      else if (meta->general->radiometry == r_GAMMA_DB)
        strcpy(radiometry, "GAMMA_DB");
      else if (meta->general->radiometry == r_POWER)
        strcpy(radiometry, "POWER");
      sprintf(str, "%s,", radiometry);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.system") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", meta->general->system);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.acquisition_date") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "\"%s\",", meta->general->acquisition_date);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.orbit") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->orbit);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.orbit_direction") == 0 &&
             dbf[ii].visible) {
      if (meta->general->orbit_direction == 'A')
        strcpy(str, "Ascending,");
      else
	strcpy(str, "Descending,");
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.frame") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->frame);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.band_count") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->band_count);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.bands") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", meta->general->bands);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.line_count") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->line_count);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.sample_count") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->sample_count);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.start_line") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->start_line);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.start_sample") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->start_sample);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.x_pixel_size") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->x_pixel_size));
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.y_pixel_size") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->y_pixel_size));
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.center_latitude") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->center_latitude));
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.center_longitude") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->center_longitude));
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.re_major") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->re_major));
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.re_minor") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->re_minor));
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.bit_error_rate") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->bit_error_rate));
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.missing_lines") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%d,", meta->general->missing_lines);
      strcat(line, str);
    }
    else if (strcmp(dbf[ii].header, "meta.general.no_data") == 0 &&
             dbf[ii].visible) {
      sprintf(str, "%s,", lf(meta->general->no_data));
      strcat(line, str);
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // SAR block
    if (meta->sar) {
      if (strcmp(dbf[ii].header, "meta.sar.image_type") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%c,", meta->sar->image_type);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.look_direction") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%c,", meta->sar->look_direction);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.look_count") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->sar->look_count);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.deskewed") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->sar->deskewed);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.original_line_count") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->sar->original_line_count);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.original_sample_count") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->sar->original_sample_count);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.line_increment") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->line_increment));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.sample_increment") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->sample_increment));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.range_time_per_pixel") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->range_time_per_pixel));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.azimuth_time_per_pixel") == 0
               && dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->azimuth_time_per_pixel));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.slant_shift") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->slant_shift));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.time_shift") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->time_shift));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.slant_range_first_pixel") == 0
               && dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->slant_range_first_pixel));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.wavelength") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->wavelength));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.prf") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->prf));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->earth_radius));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius_pp") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->earth_radius_pp));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_height") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->satellite_height));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_binary_time") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", meta->sar->satellite_binary_time);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_clock_time") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", meta->sar->satellite_clock_time);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.sar.range_doppler_coefficients") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->range_doppler_coefficients[0]));
	strcat(line, str);
        
        sprintf(str, "%s,", lf(meta->sar->range_doppler_coefficients[1]));
	strcat(line, str);
        
        sprintf(str, "%s,", lf(meta->sar->range_doppler_coefficients[2]));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.sar.azimuth_doppler_coefficients") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->azimuth_doppler_coefficients[0]));
	strcat(line, str);
        
        sprintf(str, "%s,", lf(meta->sar->azimuth_doppler_coefficients[1]));
	strcat(line, str);
        
        sprintf(str, "%s,", lf(meta->sar->azimuth_doppler_coefficients[2]));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.sar.azimuth_processing_bandwidth") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->azimuth_processing_bandwidth));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.chirp_rate") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->chirp_rate));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.pulse_duration") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->pulse_duration));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.range_sampling_rate") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->range_sampling_rate));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.polarization") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", meta->sar->polarization);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.multilook") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->sar->multilook);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.pitch") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->pitch));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.roll") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->roll));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.sar.yaw") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->sar->yaw));
        strcat(line, str);
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Optical block
    if (meta->optical) {
      if (strcmp(dbf[ii].header, "meta.optical.pointing_direction") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%s,", meta->optical->pointing_direction);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.optical.off_nadir_angle") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->optical->off_nadir_angle));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.optical.correction_level") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", meta->optical->correction_level);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.optical.cloud_percentage") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->optical->cloud_percentage));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.optical.sun_azimuth_angle") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->optical->sun_azimuth_angle));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.optical.sun_elevation_angle") == 0
               && dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->optical->sun_elevation_angle));
        strcat(line, str);
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Thermal block
    if (meta->thermal) {
      if (strcmp(dbf[ii].header, "meta.thermal.band_gain") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->thermal->band_gain));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.thermal.band_gain_change") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->thermal->band_gain_change));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.thermal.day") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->thermal->day);
        strcat(line, str);
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Transform block
    if (meta->transform) {
      int kk;
      if (strcmp(dbf[ii].header, "meta.transform.parameter_count") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%d,", meta->transform->parameter_count);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.transform.x") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          sprintf(str, "%s,", lf(meta->transform->x[kk]));
          strcat(line, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.y") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          sprintf(str, "%s,", lf(meta->transform->y[kk]));
          strcat(line, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.l") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          sprintf(str, "%s,", lf(meta->transform->l[kk]));
          strcat(line, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.s") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          sprintf(str, "%s,", lf(meta->transform->s[kk]));
          strcat(line, str);
        }
      }
      else if (strcmp(dbf[ii].header, "meta.transform.incid_a") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<6; kk++) {
          sprintf(str, "%s,", lf(meta->transform->incid_a[kk]));
          strcat(line, str);
        }
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // AirSAR block
    if (meta->airsar) {
      if (strcmp(dbf[ii].header, "meta.airsar.scale_factor") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->airsar->scale_factor));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.gps_altitude") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->airsar->gps_altitude));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.lat_peg_point") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->airsar->lat_peg_point));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.lon_peg_point") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->airsar->lon_peg_point));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.head_peg_point") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->airsar->head_peg_point));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.along_track_offset") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->airsar->along_track_offset));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.cross_track_offset") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->airsar->cross_track_offset));
        strcat(line, str);
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Projection block
    if (meta->projection) {
      if (strcmp(dbf[ii].header, "meta.projection.type") == 0 &&
          dbf[ii].visible) {
        char type[50];
        if (meta->projection->type == UNIVERSAL_TRANSVERSE_MERCATOR)
          strcpy(type, "UNIVERSAL TRANSVERSE MERCATOR");
        else if (meta->projection->type == POLAR_STEREOGRAPHIC)
          strcpy(type, "POLAR STEREOGRAPHIC");
        else if (meta->projection->type == ALBERS_EQUAL_AREA)
          strcpy(type, "ALBERS EQUAL AREA");
        else if (meta->projection->type == LAMBERT_CONFORMAL_CONIC)
          strcpy(type, "LAMBERT CONFORMAL CONIC");
        else if (meta->projection->type == LAMBERT_AZIMUTHAL_EQUAL_AREA)
          strcpy(type, "LAMBERT AZIMUTHAL EQUAL AREA");
        else if (meta->projection->type == STATE_PLANE)
          strcpy(type, "STATE PLANE");
        else if (meta->projection->type == SCANSAR_PROJECTION)
          strcpy(type, "SCANSAR PROJECTION");
        else if (meta->projection->type == LAT_LONG_PSEUDO_PROJECTION)
          strcpy(type, "LAT LONG PSEUDO PROJECTION");
        else if (meta->projection->type == UNKNOWN_PROJECTION)
          strcpy(type, "UNKNOWN PROJECTION");
        sprintf(str, "%s,", type);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.startX") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->projection->startX));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.startY") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->projection->startY));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.perX") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->projection->perX));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.perY") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->projection->perY));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.units") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", meta->projection->units);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.hem") == 0 &&
               dbf[ii].visible) {
        if (meta->projection->hem == 'N')
          strcpy(str, "North,");
        else if (meta->projection->hem == 'S')
          strcpy(str, "South,");
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.spheroid") == 0 &&
               dbf[ii].visible) {
        char spheroid[25];
        if (meta->projection->spheroid == BESSEL_SPHEROID)
          strcpy(spheroid, "BESSEL");
        else if (meta->projection->spheroid == CLARKE1866_SPHEROID)
          strcpy(spheroid, "CLARKE1866");
        else if (meta->projection->spheroid == CLARKE1880_SPHEROID)
          strcpy(spheroid, "CLARKE1880");
        else if (meta->projection->spheroid == GEM6_SPHEROID)
          strcpy(spheroid, "GEM6");
        else if (meta->projection->spheroid == GEM10C_SPHEROID)
          strcpy(spheroid, "GEM10C");
        else if (meta->projection->spheroid == GRS1980_SPHEROID)
          strcpy(spheroid, "GRS1980");
        else if (meta->projection->spheroid == INTERNATIONAL1924_SPHEROID)
          strcpy(spheroid, "INTERNATIONAL1924");
        else if (meta->projection->spheroid == INTERNATIONAL1967_SPHEROID)
          strcpy(spheroid, "INTERNATIONAL1967");
        else if (meta->projection->spheroid == WGS72_SPHEROID)
          strcpy(spheroid, "WGS72");
        else if (meta->projection->spheroid == WGS84_SPHEROID)
          strcpy(spheroid, "WGS84");
        else if (meta->projection->spheroid == HUGHES_SPHEROID)
          strcpy(spheroid, "HUGHES");
        else
          strcpy(spheroid, "UNKNOWN");
        sprintf(str, "%s,", spheroid);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.re_major") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->projection->re_major));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.re_minor") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->projection->re_minor));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.datum") == 0 &&
               dbf[ii].visible) {
        char datum[25];
        if (meta->projection->datum == EGM96_DATUM)
          strcpy(datum, "EGM96");
        else if (meta->projection->datum == ED50_DATUM)
          strcpy(datum, "ED50");
        else if (meta->projection->datum == ETRF89_DATUM)
          strcpy(datum, "ETRF89");
        else if (meta->projection->datum == ETRS89_DATUM)
          strcpy(datum, "ETRS89");
        else if (meta->projection->datum == ITRF97_DATUM)
          strcpy(datum, "ITRF97");
        else if (meta->projection->datum == NAD27_DATUM)
          strcpy(datum, "NAD27");
        else if (meta->projection->datum == NAD83_DATUM)
          strcpy(datum, "NAD83");
        else if (meta->projection->datum == WGS72_DATUM)
          strcpy(datum, "WGS72");
        else if (meta->projection->datum == WGS84_DATUM)
          strcpy(datum, "WGS84");
        else if (meta->projection->datum == HUGHES_DATUM)
          strcpy(datum, "HUGHES");
        else
          strcpy(datum, "UNKNOWN");
        sprintf(str, "%s,", datum);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.projection.height") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->projection->height));
        strcat(line, str);
      }
      if (meta->projection->type == ALBERS_EQUAL_AREA) {
        if (strcmp(dbf[ii].header,
                   "meta.projection.param.albers.std_parallel1") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", 
		  lf(meta->projection->param.albers.std_parallel1));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.std_parallel2") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", 
		  lf(meta->projection->param.albers.std_parallel2));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.center_meridian") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", 
		  lf(meta->projection->param.albers.center_meridian));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.orig_latitude") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", 
		  lf(meta->projection->param.albers.orig_latitude));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.false_easting") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", 
		  lf(meta->projection->param.albers.false_easting));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.false_northing") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,",
		  lf(meta->projection->param.albers.false_northing));
          strcat(line, str);
        }
      }
      else if (meta->projection->type == SCANSAR_PROJECTION) {
        if (strcmp(dbf[ii].header, "meta.projection.param.atct.rlocal") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.atct.rlocal));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha1") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.atct.alpha1));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha2") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.atct.alpha2));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha3") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.atct.alpha3));
          strcat(line, str);
        }
      }
      else if (meta->projection->type == LAMBERT_AZIMUTHAL_EQUAL_AREA) {
        if (strcmp(dbf[ii].header,
                   "meta.projection.param.lamaz.center_lat") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamaz.center_lat));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.center_lon") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamaz.center_lon));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.false_easting") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamaz.false_easting));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.false_northing") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", 
		  lf(meta->projection->param.lamaz.false_northing));
          strcat(line, str);
        }
      }
      else if (meta->projection->type == LAMBERT_CONFORMAL_CONIC) {
        if (strcmp(dbf[ii].header, "meta.projection.param.lamcc.plat1") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamcc.plat1));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.plat2") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamcc.plat2));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.lat0") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamcc.lat0));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.lon0") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamcc.lon0));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.false_easting") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamcc.false_easting));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.false_northing") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", 
		  lf(meta->projection->param.lamcc.false_northing));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.scale_factor") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.lamcc.scale_factor));
          strcat(line, str);
        }
      }
      else if (meta->projection->type == POLAR_STEREOGRAPHIC) {
        if (strcmp(dbf[ii].header, "meta.projection.param.ps.slat") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.ps.slat));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.slon") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.ps.slon));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.false_easting") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.ps.false_easting));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.false_northing") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.ps.false_northing));
          strcat(line, str);
        }
      }
      else if (meta->projection->type == UNIVERSAL_TRANSVERSE_MERCATOR) {
        if (strcmp(dbf[ii].header, "meta.projection.param.utm.zone") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%d,", meta->projection->param.utm.zone);
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.false_easting") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.utm.false_easting));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.false_northing") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.utm.false_northing));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.lat0") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.utm.lat0));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.lon0") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.utm.lon0));
          strcat(line, str);
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.scale_factor") == 0 &&
                 dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->projection->param.utm.scale_factor));
          strcat(line, str);
        }
      }
      else if (meta->projection->type == STATE_PLANE) {
        if (strcmp(dbf[ii].header, "meta.projection.param.state.zone") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%d,", meta->projection->param.state.zone);
          strcat(line, str);
        }
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Stats block
    if (meta->stats) {
      int kk;
      if (strcmp(dbf[ii].header, "meta.stats.band_count") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%d,", meta->stats->band_count);
        strcat(line, str);
      }
      for (kk=0; kk<meta->stats->band_count; kk++) {
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.band_id") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", meta->stats->band_stats[kk].band_id);
          strcat(line, str);
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.min") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->stats->band_stats[kk].min));
          strcat(line, str);
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.max") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->stats->band_stats[kk].max));
          strcat(line, str);
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.mean") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->stats->band_stats[kk].mean));
          strcat(line, str);
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.rmse") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->stats->band_stats[kk].rmse));
          strcat(line, str);
        }
        if (strcmp(dbf[ii].header,
                   "meta.stats.band_stats.std_deviation") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->stats->band_stats[kk].std_deviation));
          strcat(line, str);
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.mask") == 0 &&
            dbf[ii].visible) {
          sprintf(str, "%s,", lf(meta->stats->band_stats[kk].mask));
          strcat(line, str);
        }
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // State vector block
    if (meta->state_vectors) {
      int kk;
      int n = meta->state_vectors->vector_count;
      if (strcmp(dbf[ii].header, "meta.state.year") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%d,", meta->state_vectors->year);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.state.julDay") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->state_vectors->julDay);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.state.second") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->state_vectors->second));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.state.vector_count") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%d,", meta->state_vectors->vector_count);
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header, "meta.state.vectors") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<n; kk++) {
          sprintf(str, "%s,", lf(meta->state_vectors->vecs[kk].time));
          strcat(line, str);
          sprintf(str, "%s,", lf(meta->state_vectors->vecs[kk].vec.pos.x));
          strcat(line, str);
          sprintf(str, "%s,", lf(meta->state_vectors->vecs[kk].vec.pos.y));
          strcat(line, str);
          sprintf(str, "%s,", lf(meta->state_vectors->vecs[kk].vec.pos.z));
          strcat(line, str);
          sprintf(str, "%s,", lf(meta->state_vectors->vecs[kk].vec.vel.x));
          strcat(line, str);
          sprintf(str, "%s,", lf(meta->state_vectors->vecs[kk].vec.vel.y));
          strcat(line, str);
          sprintf(str, "%s,", lf(meta->state_vectors->vecs[kk].vec.vel.z));
          strcat(line, str);
        }
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Location block
    if (meta->location) {
      if (strcmp(dbf[ii].header, "meta.location.lat_start_near_range") == 0 &&
          dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lat_start_near_range));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_start_near_range") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lon_start_near_range));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lat_start_far_range") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lat_start_far_range));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_start_far_range") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lon_start_far_range));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lat_end_near_range") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lat_end_near_range));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_end_near_range") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lon_end_near_range));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lat_end_far_range") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lat_end_far_range));
        strcat(line, str);
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_end_far_range") == 0 &&
               dbf[ii].visible) {
        sprintf(str, "%s,", lf(meta->location->lon_end_far_range));
	strcat(line, str);
      }
    }
  }
  line[strlen(line)-1] = '\0';
  fprintf(fp, "%s\n", line);
  FCLOSE(fp);

  // Clean up
  FREE(header);
  FREE(line);

  return 1;
}

// Convert metadata to kml file
int meta2kml(char *inFile, char *outFile, int listFlag)
{
  FILE *fp;
  char *line = (char *) MALLOC(sizeof(char)*1024);
  int n=0;

  if (listFlag) {
    fp = FOPEN(inFile, "r");
    while (fgets(line, 1024, fp)) {
      line[strlen(line)-1] = '\0';
      meta2kml(line, outFile, 0);
      n++;
    }
  }
  else {
    meta_parameters *meta = meta_read(inFile);
    fp = FOPEN(outFile, "w");
    kml_header(fp);
    kml_entry(fp, meta, meta->general->basename);
    kml_footer(fp);
    FCLOSE(fp);
    meta_free(meta);
  }

  return 1;
}



// Convert metadata to shapefile
int meta2shape(char *inFile, char *outFile, int listFlag)
{
  DBFHandle dbase;
  SHPHandle shape;
  dbf_header_t *dbf;
  meta_parameters *meta;
  double lat[5], lon[5];
  int ii, field=0, nCols;
  
  // Read metadata
  meta = meta_read(inFile);
  int nl = meta->general->line_count;
  int ns = meta->general->sample_count;
  
  // Determine corner coordinates
  if (meta->location) {
    lat[0] = meta->location->lat_start_near_range;
    lon[0] = meta->location->lon_start_near_range;
    lat[3] = meta->location->lat_start_far_range;
    lon[3] = meta->location->lon_start_far_range;
    lat[1] = meta->location->lat_end_near_range;
    lon[1] = meta->location->lon_end_near_range;
    lat[2] = meta->location->lat_end_far_range;
    lon[2] = meta->location->lon_end_far_range;
  }
  else {
    meta_get_latLon(meta, 0, 0, 0.0, &lat[0], &lon[0]);
    meta_get_latLon(meta, 0, ns, 0.0, &lat[1], &lon[1]);
    meta_get_latLon(meta, nl, ns, 0.0, &lat[2], &lon[2]);
    meta_get_latLon(meta, nl, 0, 0.0, &lat[3], &lon[3]);
  }
  lat[4] = lat[0];
  lon[4] = lon[0];
  
  // Read configuration file
  read_header_config("META", &dbf, &nCols);

  // Initalize the database file
  shape_meta_init(outFile, meta);  
  open_shape(outFile, &dbase, &shape);

  // Write information into database file
  for (ii=0; ii<nCols; ii++) {
    // General block
    if (strcmp(dbf[ii].header, "meta.general.basename") == 0 && 
	dbf[ii].visible) {
      char *str = (char *) 
	MALLOC(sizeof(char)*strlen(meta->general->basename));
      strcpy(str, meta->general->basename);
      DBFWriteStringAttribute(dbase, 0, field, str);
      FREE(str);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.sensor") == 0 &&
	     dbf[ii].visible) {
      DBFWriteStringAttribute(dbase, 0, field, meta->general->sensor);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.sensor_name") == 0 &&
	     dbf[ii].visible) {
      DBFWriteStringAttribute(dbase, 0, field, meta->general->sensor_name);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.mode") == 0 &&
             dbf[ii].visible) {
      DBFWriteStringAttribute(dbase, 0, field, meta->general->mode);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.processor") == 0 &&
             dbf[ii].visible) {
      DBFWriteStringAttribute(dbase, 0, field, meta->general->processor);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.data_type") == 0 &&
             dbf[ii].visible) {
      char data_type[25];
      if (meta->general->data_type == BYTE)
        strcpy(data_type, "BYTE");
      else if (meta->general->data_type == INTEGER16)
        strcpy(data_type, "INTEGER16");
      else if (meta->general->data_type == INTEGER32)
        strcpy(data_type, "INTEGER32");
      else if (meta->general->data_type == REAL32)
        strcpy(data_type, "REAL32");
      else if (meta->general->data_type == REAL64)
        strcpy(data_type, "REAL64");
      else if (meta->general->data_type == COMPLEX_BYTE)
        strcpy(data_type, "COMPLEX_BYTE");
      else if (meta->general->data_type == COMPLEX_INTEGER16)
        strcpy(data_type, "COMPLEX_INTEGER16");
      else if (meta->general->data_type == COMPLEX_INTEGER32)
        strcpy(data_type, "COMPLEX_INTEGER32");
      else if (meta->general->data_type == COMPLEX_REAL32)
        strcpy(data_type, "COMPLEX_REAL32");
      else if (meta->general->data_type == COMPLEX_REAL64)
        strcpy(data_type, "COMPLEX_REAL64");
      DBFWriteStringAttribute(dbase, 0, field, data_type);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.image_data_type") == 0 &&
             dbf[ii].visible) {
      char image_data_type[25];
      if (meta->general->image_data_type == RAW_IMAGE)
        strcpy(image_data_type, "RAW_IMAGE");
      else if (meta->general->image_data_type == COMPLEX_IMAGE)
        strcpy(image_data_type, "COMPLEX_IMAGE");
      else if (meta->general->image_data_type == AMPLITUDE_IMAGE)
        strcpy(image_data_type, "AMPLITUDE_IMAGE");
      else if (meta->general->image_data_type == PHASE_IMAGE)
        strcpy(image_data_type, "PHASE_IMAGE");
      else if (meta->general->image_data_type == INTERFEROGRAM)
        strcpy(image_data_type, "INTERFEROGRAM");
      else if (meta->general->image_data_type == COHERENCE_IMAGE)
        strcpy(image_data_type, "COHERENCE_IMAGE");
      else if (meta->general->image_data_type == POLARIMETRIC_IMAGE)
        strcpy(image_data_type, "POLARIMETRIC_IMAGE");
      else if (meta->general->image_data_type == LUT_IMAGE)
        strcpy(image_data_type, "LUT_IMAGE");
      else if (meta->general->image_data_type == ELEVATION)
        strcpy(image_data_type, "ELEVATION");
      else if (meta->general->image_data_type == DEM)
        strcpy(image_data_type, "DEM");
      else if (meta->general->image_data_type == IMAGE)
        strcpy(image_data_type, "IMAGE");
      else if (meta->general->image_data_type == MASK)
        strcpy(image_data_type, "MASK");
      DBFWriteStringAttribute(dbase, 0, field, image_data_type);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.radiometry") == 0 &&
             dbf[ii].visible) {
      char radiometry[20];
      if (meta->general->radiometry == r_AMP)
        strcpy(radiometry, "AMPLITUDE");
      else if (meta->general->radiometry == r_SIGMA)
        strcpy(radiometry, "SIGMA");
      else if (meta->general->radiometry == r_BETA)
        strcpy(radiometry, "BETA");
      else if (meta->general->radiometry == r_GAMMA)
        strcpy(radiometry, "GAMMA");
      else if (meta->general->radiometry == r_SIGMA_DB)
        strcpy(radiometry, "SIGMA_DB");
      else if (meta->general->radiometry == r_BETA_DB)
        strcpy(radiometry, "BETA_DB");
      else if (meta->general->radiometry == r_GAMMA_DB)
        strcpy(radiometry, "GAMMA_DB");
      else if (meta->general->radiometry == r_POWER)
        strcpy(radiometry, "POWER");
      DBFWriteStringAttribute(dbase, 0, field, radiometry);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.system") == 0 &&
             dbf[ii].visible) {
      DBFWriteStringAttribute(dbase, 0, field, meta->general->system);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.acquisition_date") == 0 &&
             dbf[ii].visible) {
      DBFWriteStringAttribute(dbase, 0, field, 
			      meta->general->acquisition_date);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.orbit") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->orbit);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.orbit_direction") == 0 &&
             dbf[ii].visible) {
      char orbit_direction[15];
      if (meta->general->orbit_direction == 'A')
	strcpy(orbit_direction, "Ascending");
      else
       strcpy(orbit_direction, "Descending");
      DBFWriteStringAttribute(dbase, 0, field, orbit_direction);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.frame") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->frame);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.band_count") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->band_count);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.bands") == 0 &&
             dbf[ii].visible) {
      DBFWriteStringAttribute(dbase, 0, field, meta->general->bands);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.line_count") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->line_count);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.sample_count") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->sample_count);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.start_line") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->start_line);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.start_sample") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->start_sample);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.x_pixel_size") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, meta->general->x_pixel_size);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.y_pixel_size") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, meta->general->y_pixel_size);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.center_latitude") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, meta->general->center_latitude);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.center_longitude") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, 
			      meta->general->center_longitude);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.re_major") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, meta->general->re_major);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.re_minor") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, meta->general->re_minor);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.bit_error_rate") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, meta->general->bit_error_rate);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.missing_lines") == 0 &&
             dbf[ii].visible) {
      DBFWriteIntegerAttribute(dbase, 0, field, meta->general->missing_lines);
      field++;
    }
    else if (strcmp(dbf[ii].header, "meta.general.no_data") == 0 &&
             dbf[ii].visible) {
      DBFWriteDoubleAttribute(dbase, 0, field, meta->general->no_data);
      field++;
    }
  }
  for (ii=0; ii<nCols; ii++) {
    
    // SAR block
    if (meta->sar) {
      char str[12];
      if (strcmp(dbf[ii].header, "meta.sar.image_type") == 0 &&
	  dbf[ii].visible) {
	sprintf(str, "%c", meta->sar->image_type);
        DBFWriteStringAttribute(dbase, 0, field, str);
	field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.look_direction") == 0 &&
	       dbf[ii].visible) {
	sprintf(str, "%c", meta->sar->look_direction);
        DBFWriteStringAttribute(dbase, 0, field, str);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.look_count") == 0 &&
	       dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, meta->sar->look_count);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.deskewed") == 0 &&
	       dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, meta->sar->deskewed);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.original_line_count") == 0 &&
	       dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, 
				 meta->sar->original_line_count);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.original_sample_count") == 0 &&
	       dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, 
				 meta->sar->original_sample_count);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.line_increment") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->line_increment);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.sample_increment") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->sample_increment);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.range_time_per_pixel") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->sar->range_time_per_pixel);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.azimuth_time_per_pixel") == 0 
	       && dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->sar->azimuth_time_per_pixel);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.slant_shift") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->slant_shift);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.time_shift") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->time_shift);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.slant_range_first_pixel") == 0
	       && dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->sar->slant_range_first_pixel);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.wavelength") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->wavelength);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.prf") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->prf);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->earth_radius);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.earth_radius_pp") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->earth_radius_pp);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_height") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->satellite_height);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_binary_time") == 0 &&
	       dbf[ii].visible) {
	DBFWriteStringAttribute(dbase, 0, field, 
				meta->sar->satellite_binary_time);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.satellite_clock_time") == 0 &&
	       dbf[ii].visible) {
	DBFWriteStringAttribute(dbase, 0, field, 
				meta->sar->satellite_clock_time);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.sar.range_doppler_coefficients") == 0 &&
	       dbf[ii].visible) {
	DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->sar->range_doppler_coefficients[0]);
        field++;
        DBFWriteDoubleAttribute(dbase, 0, field,
				meta->sar->range_doppler_coefficients[1]);
        field++;
        DBFWriteDoubleAttribute(dbase, 0, field,
				meta->sar->range_doppler_coefficients[2]);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.sar.azimuth_doppler_coefficients") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field,
				meta->sar->azimuth_doppler_coefficients[0]);
        field++;
        DBFWriteDoubleAttribute(dbase, 0, field,
				meta->sar->azimuth_doppler_coefficients[1]);
        field++;
        DBFWriteDoubleAttribute(dbase, 0, field,
				meta->sar->azimuth_doppler_coefficients[2]);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.sar.azimuth_processing_bandwidth") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field,
				meta->sar->azimuth_processing_bandwidth);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.chirp_rate") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->chirp_rate);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.pulse_duration") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->pulse_duration);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.range_sampling_rate") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->sar->range_sampling_rate);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.polarization") == 0 &&
	       dbf[ii].visible) {
        DBFWriteStringAttribute(dbase, 0, field, meta->sar->polarization);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.multilook") == 0 &&
	       dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, meta->sar->multilook);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.pitch") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->pitch);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.roll") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->roll);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.sar.yaw") == 0 &&
	       dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->sar->yaw);
        field++;
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Optical block
    if (meta->optical) {
      if (strcmp(dbf[ii].header, "meta.optical.pointing_direction") == 0 &&
	  dbf[ii].visible) {
        DBFWriteStringAttribute(dbase, 0, field, 
				meta->optical->pointing_direction);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.optical.off_nadir_angle") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->optical->off_nadir_angle);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.optical.correction_level") == 0 &&
               dbf[ii].visible) {
        DBFWriteStringAttribute(dbase, 0, field, 
				meta->optical->correction_level);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.optical.cloud_percentage") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->optical->cloud_percentage);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.optical.sun_azimuth_angle") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->optical->sun_azimuth_angle);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.optical.sun_elevation_angle") == 0
	       && dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->optical->sun_elevation_angle);
        field++;
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Thermal block
    if (meta->thermal) {
      if (strcmp(dbf[ii].header, "meta.thermal.band_gain") == 0 &&
	  dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->thermal->band_gain);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.thermal.band_gain_change") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->thermal->band_gain_change);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.thermal.day") == 0 &&
               dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, meta->thermal->day);
        field++;
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Transform block
    if (meta->transform) {
      int kk;
      if (strcmp(dbf[ii].header, "meta.transform.parameter_count") == 0 &&
	  dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, 
				 meta->transform->parameter_count);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.transform.x") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          DBFWriteDoubleAttribute(dbase, 0, field, meta->transform->x[kk]);
	  field++;
	}
      }
      else if (strcmp(dbf[ii].header, "meta.transform.y") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          DBFWriteDoubleAttribute(dbase, 0, field, meta->transform->y[kk]);
	  field++;
	}
      }
      else if (strcmp(dbf[ii].header, "meta.transform.l") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          DBFWriteDoubleAttribute(dbase, 0, field, meta->transform->l[kk]);
	  field++;
	}
      }
      else if (strcmp(dbf[ii].header, "meta.transform.s") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<meta->transform->parameter_count; kk++) {
          DBFWriteDoubleAttribute(dbase, 0, field, meta->transform->s[kk]);
	  field++;
	}
      }
      else if (strcmp(dbf[ii].header, "meta.transform.incid_a") == 0 &&
               dbf[ii].visible) {
        for (kk=0; kk<6; kk++) {
          DBFWriteDoubleAttribute(dbase, 0, field,
				  meta->transform->incid_a[kk]);
	  field++;
	}
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // AirSAR block
    if (meta->airsar) {
      if (strcmp(dbf[ii].header, "meta.airsar.scale_factor") == 0 &&
	  dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->airsar->scale_factor);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.gps_altitude") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->airsar->gps_altitude);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.lat_peg_point") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->airsar->lat_peg_point);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.lon_peg_point") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->airsar->lon_peg_point);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.head_peg_point") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->airsar->head_peg_point);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.along_track_offset") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->airsar->along_track_offset);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.airsar.cross_track_offset") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->airsar->cross_track_offset);
        field++;
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Projection block
    if (meta->projection) {
      if (strcmp(dbf[ii].header, "meta.projection.type") == 0 &&
	  dbf[ii].visible) {
	char type[50];
        if (meta->projection->type == UNIVERSAL_TRANSVERSE_MERCATOR)
          strcpy(type, "UNIVERSAL TRANSVERSE MERCATOR");
        else if (meta->projection->type == POLAR_STEREOGRAPHIC)
          strcpy(type, "POLAR STEREOGRAPHIC");
        else if (meta->projection->type == ALBERS_EQUAL_AREA)
          strcpy(type, "ALBERS EQUAL AREA");
        else if (meta->projection->type == LAMBERT_CONFORMAL_CONIC)
          strcpy(type, "LAMBERT CONFORMAL CONIC");
        else if (meta->projection->type == LAMBERT_AZIMUTHAL_EQUAL_AREA)
          strcpy(type, "LAMBERT AZIMUTHAL EQUAL AREA");
        else if (meta->projection->type == STATE_PLANE)
          strcpy(type, "STATE PLANE");
        else if (meta->projection->type == SCANSAR_PROJECTION)
          strcpy(type, "SCANSAR PROJECTION");
        else if (meta->projection->type == LAT_LONG_PSEUDO_PROJECTION)
          strcpy(type, "LAT LONG PSEUDO PROJECTION");
        else if (meta->projection->type == UNKNOWN_PROJECTION)
          strcpy(type, "UNKNOWN PROJECTION");
	DBFWriteStringAttribute(dbase, 0, field, type);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.startX") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->projection->startX);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.startY") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->projection->startY);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.perX") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->projection->perX);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.perY") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->projection->perY);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.units") == 0 &&
               dbf[ii].visible) {
        DBFWriteStringAttribute(dbase, 0, field, meta->projection->units);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.hem") == 0 &&
               dbf[ii].visible) {
	char hemisphere[15];
        if (meta->projection->hem == 'N')
          strcpy(hemisphere, "North");
        else if (meta->projection->hem == 'S')
          strcpy(hemisphere, "South");
	DBFWriteStringAttribute(dbase, 0, field, hemisphere);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.spheroid") == 0 &&
               dbf[ii].visible) {
	char spheroid[25];
        if (meta->projection->spheroid == BESSEL_SPHEROID)
          strcpy(spheroid, "BESSEL");
        else if (meta->projection->spheroid == CLARKE1866_SPHEROID)
          strcpy(spheroid, "CLARKE1866");
        else if (meta->projection->spheroid == CLARKE1880_SPHEROID)
          strcpy(spheroid, "CLARKE1880");
        else if (meta->projection->spheroid == GEM6_SPHEROID)
          strcpy(spheroid, "GEM6");
        else if (meta->projection->spheroid == GEM10C_SPHEROID)
          strcpy(spheroid, "GEM10C");
        else if (meta->projection->spheroid == GRS1980_SPHEROID)
          strcpy(spheroid, "GRS1980");
        else if (meta->projection->spheroid == INTERNATIONAL1924_SPHEROID)
          strcpy(spheroid, "INTERNATIONAL1924");
        else if (meta->projection->spheroid == INTERNATIONAL1967_SPHEROID)
          strcpy(spheroid, "INTERNATIONAL1967");
        else if (meta->projection->spheroid == WGS72_SPHEROID)
          strcpy(spheroid, "WGS72");
        else if (meta->projection->spheroid == WGS84_SPHEROID)
          strcpy(spheroid, "WGS84");
        else if (meta->projection->spheroid == HUGHES_SPHEROID)
          strcpy(spheroid, "HUGHES");
        else
          strcpy(spheroid, "UNKNOWN");
	DBFWriteStringAttribute(dbase, 0, field, spheroid);
	field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.re_major") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->projection->re_major);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.re_minor") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->projection->re_minor);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.datum") == 0 &&
               dbf[ii].visible) {
	char datum[25];
        if (meta->projection->datum == EGM96_DATUM)
          strcpy(datum, "EGM96");
        else if (meta->projection->datum == ED50_DATUM)
          strcpy(datum, "ED50");
        else if (meta->projection->datum == ETRF89_DATUM)
          strcpy(datum, "ETRF89");
        else if (meta->projection->datum == ETRS89_DATUM)
          strcpy(datum, "ETRS89");
        else if (meta->projection->datum == ITRF97_DATUM)
          strcpy(datum, "ITRF97");
        else if (meta->projection->datum == NAD27_DATUM)
          strcpy(datum, "NAD27");
        else if (meta->projection->datum == NAD83_DATUM)
          strcpy(datum, "NAD83");
        else if (meta->projection->datum == WGS72_DATUM)
          strcpy(datum, "WGS72");
        else if (meta->projection->datum == WGS84_DATUM)
          strcpy(datum, "WGS84");
        else if (meta->projection->datum == HUGHES_DATUM)
          strcpy(datum, "HUGHES");
        else
          strcpy(datum, "UNKNOWN");
	DBFWriteStringAttribute(dbase, 0, field, datum);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.projection.height") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->projection->height);
        field++;
      }
      if (meta->projection->type == ALBERS_EQUAL_AREA) {
        if (strcmp(dbf[ii].header,
                   "meta.projection.param.albers.std_parallel1") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	     meta->projection->param.albers.std_parallel1);
	  field++;
	}
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.std_parallel2") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	    meta->projection->param.albers.std_parallel2);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.center_meridian") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	    meta->projection->param.albers.center_meridian);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.orig_latitude") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	    meta->projection->param.albers.orig_latitude);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.false_easting") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	    meta->projection->param.albers.false_easting);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.albers.false_northing") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	    meta->projection->param.albers.false_northing);
          field++;
        }
      }
      else if (meta->projection->type == SCANSAR_PROJECTION) {
        if (strcmp(dbf[ii].header, "meta.projection.param.atct.rlocal") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.atct.rlocal);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha1") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.atct.alpha1);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha2") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.atct.alpha2);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.atct.alpha3") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.atct.alpha3);
          field++;
        }
      }
      else if (meta->projection->type == LAMBERT_AZIMUTHAL_EQUAL_AREA) {
        if (strcmp(dbf[ii].header,
                   "meta.projection.param.lamaz.center_lat") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamaz.center_lat);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.center_lon") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamaz.center_lon);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.false_easting") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamaz.false_easting);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamaz.false_northing") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	    meta->projection->param.lamaz.false_northing);
          field++;
        }
      }
      else if (meta->projection->type == LAMBERT_CONFORMAL_CONIC) {
        if (strcmp(dbf[ii].header, "meta.projection.param.lamcc.plat1") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamcc.plat1);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.plat2") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamcc.plat2);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.lat0") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamcc.lat0);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.lon0") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamcc.lon0);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.false_easting") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamcc.false_easting);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.false_northing") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
	    meta->projection->param.lamcc.false_northing);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.lamcc.scale_factor") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.lamcc.scale_factor);
          field++;
        }
      }
      else if (meta->projection->type == POLAR_STEREOGRAPHIC) {
        if (strcmp(dbf[ii].header, "meta.projection.param.ps.slat") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.ps.slat);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.slon") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.ps.slon);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.false_easting") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.ps.false_easting);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.ps.false_northing") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.ps.false_northing);
          field++;
        }
      }
      else if (meta->projection->type == UNIVERSAL_TRANSVERSE_MERCATOR) {
        if (strcmp(dbf[ii].header, "meta.projection.param.utm.zone") == 0 &&
	    dbf[ii].visible) {
          DBFWriteIntegerAttribute(dbase, 0, field, 
				   meta->projection->param.utm.zone);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.false_easting") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.utm.false_easting);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.false_northing") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.utm.false_northing);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.lat0") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.utm.lat0);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.lon0") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.utm.lon0);
          field++;
        }
        else if (strcmp(dbf[ii].header,
                        "meta.projection.param.utm.scale_factor") == 0 &&
		 dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->projection->param.utm.scale_factor);
          field++;
        }
      }
      else if (meta->projection->type == STATE_PLANE) {
        if (strcmp(dbf[ii].header, "meta.projection.param.state.zone") == 0 &&
	    dbf[ii].visible) {
          DBFWriteIntegerAttribute(dbase, 0, field, 
				   meta->projection->param.state.zone);
          field++;
        }
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Stats block
    if (meta->stats) {
      int kk;
      if (strcmp(dbf[ii].header, "meta.stats.band_count") == 0 &&
	  dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, meta->stats->band_count);
	field++;
      }
      for (kk=0; kk<meta->stats->band_count; kk++) {
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.band_id") == 0 &&
	    dbf[ii].visible) {
          DBFWriteStringAttribute(dbase, 0, field, 
				  meta->stats->band_stats[kk].band_id);
	  field++;
	}
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.min") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->stats->band_stats[kk].min);
          field++;
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.max") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->stats->band_stats[kk].max);
          field++;
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.mean") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->stats->band_stats[kk].mean);
          field++;
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.rmse") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->stats->band_stats[kk].rmse);
          field++;
        }
        if (strcmp(dbf[ii].header,
		   "meta.stats.band_stats.std_deviation") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->stats->band_stats[kk].std_deviation);
          field++;
        }
        if (strcmp(dbf[ii].header, "meta.stats.band_stats.mask") == 0 &&
	    dbf[ii].visible) {
          DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->stats->band_stats[kk].mask);
          field++;
        }
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // State vector block
    if (meta->state_vectors) {
      int kk;
      int n = meta->state_vectors->vector_count;
      if (strcmp(dbf[ii].header, "meta.state.year") == 0 &&
	  dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, meta->state_vectors->year);
	field++;
      }
      else if (strcmp(dbf[ii].header, "meta.state.julDay") == 0 &&
               dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, meta->state_vectors->julDay);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.state.second") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, meta->state_vectors->second);
        field++;
      }
      else if (strcmp(dbf[ii].header, "meta.state.vector_count") == 0 &&
               dbf[ii].visible) {
        DBFWriteIntegerAttribute(dbase, 0, field, 
				 meta->state_vectors->vector_count);
	field++;
      }
      else if (strcmp(dbf[ii].header, "meta.state.vectors") == 0 &&
	       dbf[ii].visible) {
	for (kk=0; kk<n; kk++) {
	  DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->state_vectors->vecs[kk].time);
	  field++;
	  DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->state_vectors->vecs[kk].vec.pos.x);
	  field++;
	  DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->state_vectors->vecs[kk].vec.pos.y);
	  field++;
	  DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->state_vectors->vecs[kk].vec.pos.z);
	  field++;
	  DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->state_vectors->vecs[kk].vec.vel.x);
	  field++;
	  DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->state_vectors->vecs[kk].vec.vel.y);
	  field++;
	  DBFWriteDoubleAttribute(dbase, 0, field, 
				  meta->state_vectors->vecs[kk].vec.vel.z);
	  field++;
	}
      }
    }
  }
  for (ii=0; ii<nCols; ii++) {
    // Location block
    if (meta->location) {
      if (strcmp(dbf[ii].header, "meta.location.lat_start_near_range") == 0 &&
	  dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lat_start_near_range);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_start_near_range") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lon_start_near_range);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lat_start_far_range") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lat_start_far_range);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_start_far_range") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lon_start_far_range);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lat_end_near_range") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lat_end_near_range);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_end_near_range") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lon_end_near_range);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lat_end_far_range") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lat_end_far_range);
        field++;
      }
      else if (strcmp(dbf[ii].header,
                      "meta.location.lon_end_far_range") == 0 &&
               dbf[ii].visible) {
        DBFWriteDoubleAttribute(dbase, 0, field, 
				meta->location->lon_end_far_range);
        field++;
      }
    }
  }
  
  // Write shape object
  SHPObject *shapeObject=NULL;
  shapeObject = SHPCreateSimpleObject(SHPT_POLYGON, 5, lon, lat, NULL);
  SHPWriteObject(shape, -1, shapeObject);
  SHPDestroyObject(shapeObject);
  
  // Clean up
  meta_free(meta);
  close_shape(dbase, shape);
  write_esri_proj_file(outFile);

  return 1;
}

// Convert leader file to metadata - just a simple read and write actually
int leader2meta(char *inFile, char *outFile, int listFlag)
{
  FILE *fp;
  meta_parameters *meta;
  char *line = (char *) MALLOC(sizeof(char)*1024);
  int n=0;

  if (listFlag) {
    fp = FOPEN(inFile, "r");
    while (fgets(line, 1024, fp)) {
      line[strlen(line)-1] = '\0';
      meta = meta_read(inFile);
      meta_write(meta, outFile);
      n++;
    }
    FCLOSE(fp);
  }
  else {
    meta = meta_read(inFile);
    meta_write(meta, outFile);
  }

  return 1;
}