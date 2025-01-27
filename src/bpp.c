/*
    Copyright (C) 2016-2019 Tomas Flouri and Ziheng Yang

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact: Tomas Flouri <t.flouris@ucl.ac.uk>,
    Department of Genetics, Evolution and Environment,
    University College London, Gower Street, London WC1E 6BT, England
*/

#include "bpp.h"

#ifdef _MSC_VER
#include "getopt_win.h"
#endif

static char * progname;
static char progheader[80];
char * cmdline;

/* global error message buffer */
__THREAD int bpp_errno;
__THREAD char bpp_errmsg[200] = {0};

/* options */
long opt_alpha_cats;
long opt_arch;
long opt_basefreqs_fixed;
long opt_burnin;
long opt_checkpoint;
long opt_checkpoint_current;
long opt_checkpoint_initial;
long opt_checkpoint_step;
long opt_cleandata;
long opt_clock;
long opt_comply;
long opt_constraint_count;
long opt_debug;
long opt_debug_full;
long opt_debug_gspr;
long opt_debug_rates;
long opt_debug_expand_count;
long opt_debug_expshr_count;
long opt_debug_shrink_count;
long opt_debug_start;
long opt_debug_end;
long opt_debug_abort;

long opt_debug_sim;
long opt_debug_gage;
long opt_debug_gspr;
long opt_debug_mui;
long opt_debug_hs;
long opt_debug_mix;
long opt_debug_rj;
long opt_debug_theta;
long opt_debug_tau;
long opt_debug_sspr;
long opt_debug_snl;
long opt_debug_br;
long opt_debug_bruce;
long opt_debug_parser;
long opt_debug_counter;

long opt_delimit_prior;
long opt_diploid_size;
long opt_est_delimit;
long opt_est_heredity;
long opt_est_locusrate;
long opt_est_mubar;
long opt_est_stree;
long opt_est_theta;
long opt_exp_randomize;
long opt_exp_theta;
long opt_exp_sim;
long opt_finetune_reset;
long opt_help;
long opt_load_balance;
long opt_locusrate_prior;
long opt_locus_count;
long opt_locus_simlen;
long opt_max_species_count;
long opt_method;
long opt_migration;
long opt_model;
long opt_msci;
long opt_onlysummary;
long opt_partition_count;
long opt_print_genetrees;
long opt_print_hscalars;
long opt_print_locusfile;
long opt_print_locusrate;
long opt_print_qmatrix;
long opt_print_rates;
long opt_print_samples;
long opt_qrates_fixed;
long opt_quiet;
long opt_rate_prior;
long opt_revolutionary_spr_method;
long opt_revolutionary_spr_debug;
long opt_rev_gspr;
long opt_rjmcmc_method;
long opt_samplefreq;
long opt_samples;
long opt_scaling;
long opt_seed;
long opt_siterate_fixed;
long opt_siterate_cats;
long opt_tau_dist;
long opt_theta_dist;
long opt_threads;
long opt_threads_start;
long opt_threads_step;
long opt_usedata;
long opt_version;
double opt_alpha_alpha;
double opt_alpha_beta;
double opt_bfbeta;
double opt_clock_vbar;
double opt_finetune_alpha;
double opt_finetune_branchrate;
double opt_finetune_freqs;
double opt_finetune_gtage;
double opt_finetune_gtspr;
double opt_finetune_locusrate;
double opt_finetune_mix;
double opt_finetune_mubar;
double opt_finetune_mui;
double opt_finetune_phi;
double opt_finetune_qrates;
double opt_finetune_nubar;
double opt_finetune_nui;
double opt_finetune_tau;
double opt_finetune_theta;
double opt_heredity_alpha;
double opt_heredity_beta;
double opt_locusrate_mubar;
double opt_mubar_alpha;
double opt_mubar_beta;
double opt_mui_alpha;
double opt_prob_snl;
double opt_prob_snl_shrink;
double opt_phi_alpha;
double opt_phi_beta;
double opt_rjmcmc_alpha;
double opt_rjmcmc_epsilon;
double opt_rjmcmc_mean;
double opt_siterate_alpha;
double opt_siterate_beta;
double opt_snl_lambda_expand;
double opt_snl_lambda_shrink;
double opt_tau_alpha;
double opt_tau_beta;
double opt_theta_alpha;
double opt_theta_beta;
double opt_theta_max;
double opt_theta_min;
double opt_theta_p;
double opt_theta_q;
double opt_vbar_alpha;
double opt_vbar_beta;
double opt_vi_alpha;
long * opt_diploid;
long * opt_sp_seqcount;
char * opt_cfile;
char * opt_concatfile;
char * opt_constraintfile;
char * opt_heredity_filename;
char * opt_locusrate_filename;
char * opt_mapfile;
char * opt_mcmcfile;
char * opt_modelparafile;
char * opt_msafile;
char * opt_mscifile;
char * opt_outfile;
char * opt_partition_file;
char * opt_reorder;
char * opt_resume;
char * opt_simulate;
char * opt_streenewick;
char * opt_treefile;
double * opt_basefreqs_params;
double * opt_migration_events;
double * opt_migration_matrix;
double * opt_qrates_params;
char ** opt_migration_labels;
partition_t ** opt_partition_list;

long mmx_present;
long sse_present;
long sse2_present;
long sse3_present;
long ssse3_present;
long sse41_present;
long sse42_present;
long popcnt_present;
long avx_present;
long avx2_present;
long altivec_present;

static struct option long_options[] =
{
  {"help",         no_argument,       0, 0 },  /*  0 */
  {"version",      no_argument,       0, 0 },  /*  1 */
  {"quiet",        no_argument,       0, 0 },  /*  2 */
  {"cfile",        required_argument, 0, 0 },  /*  3 */
  {"arch",         required_argument, 0, 0 },  /*  4 */
  {"exp_method",   required_argument, 0, 0 },  /*  5 */
  {"exp_debug",    no_argument,       0, 0 },  /*  6 */
  {"resume",       required_argument, 0, 0 },  /*  7 */
  {"simulate",     required_argument, 0, 0 },  /*  8 */
  {"exp_random",   no_argument,       0, 0 },  /*  9 */
  {"rev_gspr",     no_argument,       0, 0 },  /* 10 */
  {"debugrates",   no_argument,       0, 0 },  /* 11 */
  {"msci-create",  required_argument, 0, 0 },  /* 12 */
  {"comply",       no_argument,       0, 0 },  /* 13 */
  {"tree",         required_argument, 0, 0 },  /* 14 */
  {"constraint",   required_argument, 0, 0 },  /* 15 */
  {"full",         no_argument,       0, 0 },  /* 16 */
  {"debug",        optional_argument, 0, 0 },  /* 17 */
  {"debug_gage",   optional_argument, 0, 0 },  /* 18 */
  {"debug_gspr",   optional_argument, 0, 0 },  /* 19 */
  {"debug_mui",    optional_argument, 0, 0 },  /* 20 */
  {"debug_hs",     optional_argument, 0, 0 },  /* 21 */
  {"debug_mix",    optional_argument, 0, 0 },  /* 22 */
  {"debug_rj",     optional_argument, 0, 0 },  /* 23 */
  {"debug_theta",  optional_argument, 0, 0 },  /* 24 */
  {"debug_tau",    optional_argument, 0, 0 },  /* 25 */
  {"debug_sspr",   optional_argument, 0, 0 },  /* 26 */
  {"debug_br",     optional_argument, 0, 0 },  /* 27 */
  {"debug_snl",    optional_argument, 0, 0 },  /* 28 */
  {"debug_parser", optional_argument, 0, 0 },  /* 29 */
  {"debug_start",  required_argument, 0, 0 },  /* 30 */
  {"debug_end",    required_argument, 0, 0 },  /* 31 */
  {"debug_abort",  required_argument, 0, 0 },  /* 32 */
  {"exp_theta",    no_argument,       0, 0 },  /* 33 */
  {"debug_bruce",  no_argument,       0, 0 },  /* 34 */
  {"exp_sim",      no_argument,       0, 0 },  /* 35 */
  {"summary",      required_argument, 0, 0 },  /* 36 */
  { 0, 0, 0, 0 }
};

const char * global_model_strings[28] =
 {
   "JC69",                          /*  0 */
   "K80",                           /*  1 */
   "F81",                           /*  2 */
   "HKY",                           /*  3 */
   "T92",                           /*  4 */
   "TN93",                          /*  5 */
   "F84",                           /*  6 */
   "GTR",                           /*  7 */
   "CUSTOM",                        /*  8 */
   "DAYHOFF",                       /*  9 */
   "LG",                            /* 10 */
   "DCMUT",                         /* 11 */
   "JTT",                           /* 12 */
   "MTREV",                         /* 13 */
   "WAG",                           /* 14 */
   "RTREV",                         /* 15 */
   "CPREV",                         /* 16 */
   "VT",                            /* 17 */
   "BLOSUM62",                      /* 18 */
   "MTMAM",                         /* 19 */
   "MTART",                         /* 20 */
   "MTZOA",                         /* 21 */
   "PMB",                           /* 22 */
   "HIVB",                          /* 23 */
   "HIVW",                          /* 24 */
   "JTTDCMUT",                      /* 25 */
   "FLU",                           /* 26 */
   "STMTREV"                        /* 27 */
 };

const char * global_freqs_strings[28] =
 {
   "Fixed",                                 /*  0 JC69     */
   "Fixed",                                 /*  1 K80      */
   "Empirical ACGT X.XX X.XX X.XX X.XX",    /*  2 F81      */
   "Empirical ACGT X.XX X.XX X.XX X.XX",    /*  3 HKY      */
   "Empirical GC X.XX",                     /*  4 T92      */
   "Empirical ACGT X.XX X.XX X.XX X.XX",    /*  5 TN93     */
   "Empirical ACGT X.XX X.XX X.XX X.XX",    /*  6 F84      */
   "Empirical ACGT X.XX X.XX X.XX X.XX",    /*  7 GTR      */
   "CUSTOM",                                /*  8 CUSTOM   */
   "Fixed",                                 /*  9 DAYHOFF  */
   "Fixed",                                 /* 10 LG       */
   "Fixed",                                 /* 11 DCMUT    */
   "Fixed",                                 /* 12 JTT      */
   "Fixed",                                 /* 13 MTREV    */
   "Fixed",                                 /* 14 WAG      */
   "Fixed",                                 /* 15 RTREV    */
   "Fixed",                                 /* 16 CPREV    */
   "Fixed",                                 /* 17 VT       */
   "Fixed",                                 /* 18 BLOSUM62 */
   "Fixed",                                 /* 19 MTMAM    */
   "Fixed",                                 /* 20 MTART    */
   "Fixed",                                 /* 21 MTZOA    */
   "Fixed",                                 /* 22 PMB      */
   "Fixed",                                 /* 23 HIVB     */
   "Fixed",                                 /* 24 HIVW     */
   "Fixed",                                 /* 25 JTTDCMUT */
   "Fixed",                                 /* 26 FLU      */
   "Fixed"                                  /* 27 STMTREV  */
 };

static long args_getlong(char * arg)
{
  int len = 0;
  long temp;

  int ret = sscanf(arg, "%ld%n", &temp, &len);
  if ((ret == 0) || (((unsigned int)(len)) < strlen(arg)))
    fatal("Illegal option argument");
  return temp;
}

#if 0
static double args_getdouble(char * arg)
{
  int len = 0;
  double temp = 0;
  int ret = sscanf(arg, "%lf%n", &temp, &len);
  if ((ret == 0) || (((unsigned int)(len)) < strlen(arg)))
    fatal("Illegal option argument");
  return temp;
}
#endif

void args_init(int argc, char ** argv)
{
  int option_index = 0;
  int c;
  
  /* set defaults */

  progname = argv[0];

  opt_alpha_alpha = 1;
  opt_alpha_beta = 2;
  opt_alpha_cats = 1;
  opt_arch = -1;
  opt_basefreqs_fixed = -1;
  opt_basefreqs_params = NULL;
  opt_bfbeta = 1;
  opt_mubar_alpha = -1;
  opt_mubar_beta = -1;
  opt_mui_alpha = -1;
  opt_vbar_alpha = -1;
  opt_vbar_beta = -1;
  opt_vi_alpha = -1;
  opt_burnin = 100;
  opt_cfile = NULL;
  opt_clock = BPP_CLOCK_GLOBAL;
  opt_clock_vbar = 0;

  opt_checkpoint = 0;
  opt_checkpoint_initial = 0;
  opt_checkpoint_current = 0;
  opt_checkpoint_step = 0;
  opt_cleandata = 0;
  opt_comply = 0;
  opt_concatfile = NULL;
  opt_constraintfile = NULL;
  opt_constraint_count = 0;
  opt_debug = 0;
  opt_debug_abort = 0;
  opt_debug_br = 0;
  opt_debug_bruce = 0;
  opt_debug_counter = 0;
  opt_debug_end = 0;
  opt_debug_expand_count = 0;
  opt_debug_expshr_count = 0;
  opt_debug_full = 0;
  opt_debug_hs = 0;
  opt_debug_gage = 0;
  opt_debug_gspr = 0;
  opt_debug_mix = 0;
  opt_debug_mui = 0;
  opt_debug_parser = 0;
  opt_debug_rates = 0;
  opt_debug_rj = 0;
  opt_debug_shrink_count = 0;
  opt_debug_sim = 0;
  opt_debug_snl = 0;
  opt_debug_sspr = 0;
  opt_debug_start = 0;
  opt_debug_tau = 0;
  opt_debug_theta = 0;
  opt_delimit_prior = BPP_SPECIES_PRIOR_UNIFORM;
  opt_diploid = NULL;
  opt_diploid_size = 0;
  opt_est_delimit = 0;
  opt_est_heredity = 0;
  opt_est_locusrate = MUTRATE_CONSTANT;
  opt_est_mubar = 0;
  opt_est_stree = 0;
  opt_est_theta = 1;
  opt_exp_randomize = 0;
  opt_exp_theta = 0;
  opt_exp_sim = 0;
#if(0)
  opt_revolutionary_spr_method = 1;
  opt_revolutionary_spr_debug = 2;
#else
  opt_revolutionary_spr_method = 0;
  opt_revolutionary_spr_debug = 0;
#endif
  opt_finetune_alpha = 0.1;
  opt_finetune_branchrate = 0.1;
  opt_finetune_freqs = 0.1;
  opt_finetune_gtage = 5;
  opt_finetune_gtspr = 0.001;
  opt_finetune_locusrate = 0.33;
  opt_finetune_mix = 0.3;
  opt_finetune_mui = 0.1;
  opt_finetune_mubar = 0.1;
  opt_finetune_phi = 0.001;
  opt_finetune_qrates = 0.3;
  opt_finetune_reset = 0;
  opt_finetune_nubar = 0.1;
  opt_finetune_nui = 0.1;
  opt_finetune_tau = 0.001;
  opt_finetune_theta = 0.001;
  opt_help = 0;
  opt_heredity_alpha = 0;
  opt_heredity_beta = 0;
  opt_heredity_filename = NULL;
  opt_load_balance = BPP_LB_ZIGZAG;
  opt_locusrate_filename = NULL;
  opt_locusrate_prior = -1;
  opt_locusrate_mubar = 1;
  opt_locus_count = 0;
  opt_locus_simlen = 0;
  opt_mapfile = NULL;
  opt_max_species_count = 0;
  opt_mcmcfile = NULL;
  opt_method = -1;
  opt_migration = 0;
  opt_migration_events = NULL;
  opt_migration_labels = NULL;
  opt_migration_matrix = NULL;
  opt_model = -1;
  opt_modelparafile = NULL;
  opt_msafile = NULL;
  opt_msci = 0;
  opt_mscifile = NULL;
  opt_onlysummary = 0;
  opt_outfile = NULL;
  opt_partition_count = 0;
  opt_partition_file = NULL;
  opt_partition_list = NULL;
  opt_phi_alpha = 0;
  opt_phi_beta = 0;
  opt_print_genetrees = 0;
  opt_print_hscalars = 0;
  opt_print_locusfile = 0;
  opt_print_locusrate = 0;
  opt_print_qmatrix = 0;
  opt_print_rates = 0;
  opt_print_samples = 1;
  opt_prob_snl = 0.2;
  opt_prob_snl_shrink = 0.333;
  opt_qrates_fixed = -1;
  opt_qrates_params = NULL;
  opt_quiet = 0;
  opt_rate_prior = BPP_BRATE_PRIOR_GAMMA;
  opt_resume = NULL;
  opt_rev_gspr = 0;
  opt_rjmcmc_alpha = -1;
  opt_rjmcmc_epsilon = -1;
  opt_rjmcmc_mean = -1;
  opt_rjmcmc_method = -1;
  opt_samplefreq = 10;
  opt_samples = 0;
  opt_scaling = 0;
  opt_seed = -1;
  opt_simulate = NULL;
  opt_siterate_fixed = 1;
  opt_siterate_alpha = 0;
  opt_siterate_beta = 0;
  opt_siterate_cats = 5;
  opt_snl_lambda_expand = 0.1;
  opt_snl_lambda_shrink = 0.2;
  opt_sp_seqcount = NULL;
  opt_streenewick = NULL;
  opt_tau_alpha = 0;
  opt_tau_beta = 0;
  opt_tau_dist = BPP_TAU_PRIOR_INVGAMMA;
  opt_theta_alpha = 0;
  opt_theta_beta = 0;
  opt_theta_dist = BPP_THETA_PRIOR_INVGAMMA;
  opt_theta_max = 0;
  opt_theta_min = 0;
  opt_theta_p = 0;
  opt_theta_q = 0;
  opt_threads = 1;
  opt_threads_start = 1;
  opt_threads_step = 1;
  opt_treefile = NULL;
  opt_usedata = 1;
  opt_version = 0;

  while ((c = getopt_long_only(argc, argv, "", long_options, &option_index)) == 0)
  {
    switch (option_index)
    {
      case 0:
        opt_help = 1;
        break;

      case 1:
        opt_version = 1;
        break;

      case 2:
        opt_quiet = 1;
        break;

      case 3:
        opt_cfile = xstrdup(optarg);
        break;

      case 4:
        if (!strcasecmp(optarg,"cpu"))
          opt_arch = PLL_ATTRIB_ARCH_CPU;
        else if (!strcasecmp(optarg,"sse"))
          opt_arch = PLL_ATTRIB_ARCH_SSE;
        else if (!strcasecmp(optarg,"avx"))
          opt_arch = PLL_ATTRIB_ARCH_AVX;
        else if (!strcasecmp(optarg,"avx2"))
          opt_arch = PLL_ATTRIB_ARCH_AVX2;
        else
          fatal("Invalid instruction set (%s)", optarg);
        break;

      case 5:
        opt_revolutionary_spr_method = args_getlong(optarg);
        break;

      case 6:
        opt_revolutionary_spr_debug = 1;
        break;

      case 7:
        opt_resume = optarg;
        break;

      case 8:
        opt_simulate = optarg;
        break;

      case 9:
        opt_exp_randomize = 1;
        break;

      case 10:
        opt_rev_gspr = 1;
        break;

      case 11:
        opt_debug_rates = 1;
        break;

      case 12:
        opt_mscifile = xstrdup(optarg);
        break;

      case 13:
        opt_comply = 1;
        break;

      case 14:
        opt_treefile = xstrdup(optarg);
        break;

      case 15:
        opt_constraintfile = xstrdup(optarg);
        break;

      case 16:
        opt_debug_full = 1;
        break;

      case 17:
        opt_debug = 1;
        if (optarg)
          opt_debug = atol(optarg);
        break;

      case 18:
        opt_debug_gage = 1;
        if (optarg)
          opt_debug_gage = atol(optarg);
        break;

      case 19:
        opt_debug_gspr = 1;
        if (optarg)
          opt_debug_gspr = atol(optarg);
        break;

      case 20:
        opt_debug_mui = 1;
        if (optarg)
          opt_debug_mui = atol(optarg);
        break;

      case 21:
        opt_debug_hs = 1;
        if (optarg)
          opt_debug_hs = atol(optarg);
        break;

      case 22:
        opt_debug_mix = 1;
        if (optarg)
          opt_debug_mix = atol(optarg);
        break;

      case 23:
        opt_debug_rj = 1;
        if (optarg)
          opt_debug_rj = atol(optarg);
        break;

      case 24:
        opt_debug_theta = 1;
        if (optarg)
          opt_debug_theta = atol(optarg);
        break;

      case 25:
        opt_debug_tau = 1;
        if (optarg)
          opt_debug_tau = atol(optarg);
        break;

      case 26:
        opt_debug_sspr = 1;
        if (optarg)
          opt_debug_sspr = atol(optarg);
        break;

      case 27:
        opt_debug_br = 1;
        if (optarg)
          opt_debug_br = atol(optarg);
        break;

      case 28:
        opt_debug_snl = 1;
        if (optarg)
          opt_debug_snl = atol(optarg);
        break;

      case 29:
        opt_debug_parser = 1;
        if (optarg)
          opt_debug_parser = atol(optarg);
        break;

      case 30:
        opt_debug_start = atol(optarg);
        break;

      case 31:
        opt_debug_end = atol(optarg);
        break;

      case 32:
        opt_debug_abort = atol(optarg);
        break;

      case 33:
        opt_exp_theta = 1;
        break;

      case 34:
        opt_debug_bruce = 1;
        break;

      case 35:
        opt_exp_sim = 1;
        break;

      case 36:
        opt_cfile = xstrdup(optarg);
        opt_onlysummary = 1;
        break;

      default:
        fatal("Internal error in option parsing");
    }
  }

  if (c != -1)
    exit(EXIT_FAILURE);

  int commands  = 0;

  if (opt_cfile)
  {
    opt_model = BPP_DNA_MODEL_DEFAULT;
    load_cfile();
  }
  if (opt_simulate)
    load_cfile_sim();

  /* check for number of independent commands selected */
  if (opt_version)
    commands++;
  if (opt_help)
    commands++;
  if (opt_cfile)
    commands++;
  if (opt_resume)
    commands++;
  if (opt_simulate)
    commands++;
  if (opt_mscifile)
    commands++;
  if (opt_comply)
    commands++;

  /* if more than one independent command, fail */
  if (commands > 1)
    fatal("More than one command specified");

  if (opt_prob_snl_shrink <= 0 || opt_prob_snl_shrink >= 1)
    fatal("Proportion of SHRINK moves must be between 0 and 1");

  /* if no command specified, turn on --help */
  if (!commands)
  {
    opt_help = 1;
    return;
  }
}

static void dealloc_switches()
{
  if (opt_cfile) free(opt_cfile);
  if (opt_constraintfile) free(opt_constraintfile);
  if (opt_mapfile) free(opt_mapfile);
  if (opt_mcmcfile) free(opt_mcmcfile);
  if (opt_msafile) free(opt_msafile);
  if (opt_mscifile) free(opt_mscifile);
  if (opt_outfile) free(opt_outfile);
  if (opt_reorder) free(opt_reorder);
  if (opt_sp_seqcount) free(opt_sp_seqcount);
  if (opt_streenewick) free(opt_streenewick);

  /* mccoal switches */
  if (opt_basefreqs_params) free(opt_basefreqs_params);
  if (opt_concatfile) free(opt_concatfile);
  if (opt_modelparafile) free(opt_modelparafile);
  if (opt_qrates_params) free(opt_qrates_params);
  if (opt_treefile) free(opt_treefile);
}

void cmd_help()
{
  /*         0         1         2         3         4         5         6         7          */
  /*         01234567890123456789012345678901234567890123456789012345678901234567890123456789 */

  fprintf(stderr,
          "Usage: %s [OPTIONS]\n", progname);
  fprintf(stderr,
          "\n"
          "General options:\n"
          "  --help             display help information\n"
          "  --version          display version information\n"
          "  --quiet            only output warnings and fatal errors to stderr\n"
          "  --cfile FILENAME   run analysis for the specified control file\n"
          "  --resume FILENAME  resume analysis from a specified checkpoint file\n"
          "  --arch SIMD        force specific vector instruction set (default: auto)\n"
          "\n"
         );

  /*         0         1         2         3         4         5         6         7          */
  /*         01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

void getentirecommandline(int argc, char * argv[])
{
  int len = 0;
  int i;

  for (i = 0; i < argc; ++i)
    len += strlen(argv[i]);

  cmdline = (char *)xmalloc((size_t)(len + argc + 1));
  cmdline[0] = 0;

  for (i = 0; i < argc; ++i)
  {
    strcat(cmdline, argv[i]);
    strcat(cmdline, " ");
  }
}

void fillheader()
{
  snprintf(progheader, 80,
           "%s %s_%s, %1.fGB RAM, %ld cores",
           PROG_NAME, PROG_VERSION, PROG_ARCH,
           arch_get_memtotal() / 1024.0 / 1024.0 / 1024.0,
           arch_get_cores());
}

void show_header()
{
  fprintf(stdout, "%s\n", progheader);
  fprintf(stdout, "https://github.com/bpp/bpp\n");
  fprintf(stdout,"\n");
}

int main (int argc, char * argv[])
{
  fillheader();
  getentirecommandline(argc, argv);

  args_init(argc, argv);

  show_header();

  cpu_features_detect();
  cpu_features_show();
  if (!opt_version && !opt_help)
    cpu_setarch();

  /* intiialize random number generators */
  #ifdef DEBUG_THREADS
  /* simulate the results of DEBUG_THREADS_COUNT threads with a single thread.
     This part is to init DEBUG_THREADS_COUNT random number generators */
  if (opt_threads == 1)
  {
    opt_threads = DEBUG_THREADS_COUNT;
    legacy_init();
    opt_threads = 1;
  }
  else
    legacy_init();
  #else
  legacy_init();
  #endif

  if (opt_help)
  {
    cmd_help();
  }
  else if (opt_version)
  {
    ;
  }
  else if (opt_resume || opt_cfile)
  {
    cmd_run();
    //d_mcmc();
  }
  else if (opt_simulate)
  {
    cmd_simulate();
  }
  else if (opt_mscifile)
  {
    cmd_msci_create();
  }
  else if (opt_comply)
  {
    cmd_comply();
  }

  legacy_fini();
  dealloc_switches();
  free(cmdline);
  return (0);
}
