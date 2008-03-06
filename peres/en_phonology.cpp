
#include "en_phonology.h"
#include "tactics.h"
#include "misc.h"

typedef Distribution<Segment> SegmentDistribution;

// monophthongs
static Phoneme sv_i (SHORTVOWEL_I,					SHORT_VOWEL);
static Phoneme sv_u (SHORTVOWEL_U,					SHORT_VOWEL);
static Phoneme sv_e0(SHORTVOWEL_MID_CENTRAL_E,		SHORT_VOWEL);
static Phoneme sv_e1(SHORTVOWEL_OPENMID_FRONT_E,	SHORT_VOWEL);
static Phoneme sv_a0(SHORTVOWEL_OPEN_FRONT_A,		SHORT_VOWEL);
static Phoneme sv_a1(SHORTVOWEL_OPEN_CENTRAL_A,	SHORT_VOWEL);
static Phoneme sv_o0(SHORTVOWEL_OPENMID_BACK_O,	SHORT_VOWEL);
static Phoneme lv_i(LONGVOWEL_I, 					LONG_VOWEL);
static Phoneme lv_u(LONGVOWEL_U, 					LONG_VOWEL);
static Phoneme lv_e(LONGVOWEL_E, 					LONG_VOWEL);
static Phoneme lv_o(LONGVOWEL_O, 					LONG_VOWEL);
static Phoneme lv_a(LONGVOWEL_A, 					LONG_VOWEL);

// vowel phonemes only occurring in diphthongs
static Phoneme sv_e2(SHORTVOWEL_MID_FRONT_E,		SHORT_VOWEL);
static Phoneme sv_o1(SHORTVOWEL_CLOSEMID_BACK_O,	SHORT_VOWEL);
static Phoneme sv_a4(SHORTVOWEL_OPEN_FRONT_A,		SHORT_VOWEL);
static Phoneme schwa(SCHWA,						SHORT_VOWEL);

// consonant phonemes
static Phoneme c_p(CONSONANT_P,					PLOSIVE 	| VOICELESS | BILABIAL);
static Phoneme c_b(CONSONANT_B,					PLOSIVE 	| VOICED 	| BILABIAL);
static Phoneme c_t(CONSONANT_T,					PLOSIVE 	| VOICELESS | ALVEOLAR);
static Phoneme c_d(CONSONANT_D,					PLOSIVE 	| VOICED 	| ALVEOLAR);
static Phoneme c_k(CONSONANT_K,					PLOSIVE 	| VOICELESS | VELAR);
static Phoneme c_g(CONSONANT_G,					PLOSIVE 	| VOICED 	| VELAR);
static Phoneme c_m(CONSONANT_M,					NASAL					| BILABIAL);
static Phoneme c_n(CONSONANT_N,					NASAL					| ALVEOLAR);
static Phoneme c_ng(CONSONANT_NG,				NASAL					| VELAR);
static Phoneme c_f(CONSONANT_F,					FRICATIVE 	| VOICELESS | LABIODENTAL);
static Phoneme c_v(CONSONANT_V,					FRICATIVE 	| VOICED 	| LABIODENTAL);
static Phoneme c_th0(CONSONANT_TH0,				FRICATIVE 	| VOICELESS | DENTAL);
static Phoneme c_th1(CONSONANT_TH1,				FRICATIVE 	| VOICED 	| DENTAL);
static Phoneme c_s(CONSONANT_S,					FRICATIVE 	| VOICELESS | ALVEOLAR);
static Phoneme c_z(CONSONANT_Z,					FRICATIVE 	| VOICED 	| ALVEOLAR);
static Phoneme c_sh(CONSONANT_SH,				FRICATIVE 	| VOICELESS | POSTALVEOLAR);
static Phoneme c_zh(CONSONANT_ZH,				FRICATIVE 	| VOICED 	| POSTALVEOLAR);
static Phoneme c_h(CONSONANT_H,					FRICATIVE 				| GLOTTAL);
static Phoneme c_ch(CONSONANT_CH,				AFFRICATE 	| VOICELESS | POSTALVEOLAR);
static Phoneme c_dj(CONSONANT_DJ,				AFFRICATE 	| VOICED 	| POSTALVEOLAR);
static Phoneme c_r(CONSONANT_R,					APPROXIMANT 			| ALVEOLAR);
static Phoneme c_j(CONSONANT_J,					APPROXIMANT 			| PALATAL);
static Phoneme c_w(CONSONANT_W,					APPROXIMANT 			| LABIOVELAR);
static Phoneme c_l(CONSONANT_L,					LATERAL				 	| ALVEOLAR);

Phoneme phonemes[] = {
	sv_i ,
	sv_u ,
	sv_e0,
	sv_e1,
	sv_a0,
	sv_a1,
	sv_o0,
	lv_i,
	lv_u,
	lv_e,
	lv_o,
	lv_a,
	// vowel phonemes only occurring in diphthongs,
	sv_e2,
	sv_o1,
	sv_a4,
	schwa,
	// consonant phonemes,
	c_p,
	c_b,
	c_t,
	c_d,
	c_k,
	c_g,
	c_m,
	c_n,
	c_ng,
	c_f,
	c_v,
	c_th0,
	c_th1,
	c_s,
	c_z,
	c_sh,
	c_zh,
	c_h,
	c_ch,
	c_dj,
	c_r,
	c_j,
	c_w,
	c_l
};

// vowel nuclei
static Segment seg_sv_i( "i", sv_i);
static Segment seg_sv_u( "u", sv_u);
static Segment seg_sv_e0( "e", sv_e0);
static Segment seg_sv_e1( "e", sv_e1);
static Segment seg_sv_a0( "a", sv_a0);
static Segment seg_sv_a1( "a", sv_a1);
static Segment seg_sv_o( "o", sv_o0);
static Segment seg_lv_i( "i", lv_i);
static Segment seg_lv_u( "u", lv_u);
static Segment seg_lv_e( "e", lv_e);
static Segment seg_lv_o( "o", lv_o);
static Segment seg_lv_a( "a", lv_a);
static Segment seg_diph_ei( "ei",  sv_e2, sv_i );
static Segment seg_diph_ou( "ou",  sv_o1, sv_u );
static Segment seg_diph_ai( "ai",  sv_a4, sv_i );
static Segment seg_diph_au( "au",  sv_a4, sv_u );
static Segment seg_diph_oi( "oi",  sv_o0, sv_i );
static Segment seg_diph_uschwa( "u",  sv_u, schwa );
static Segment seg_diph_eschwa( "e",  sv_e1, schwa );

// consonant clusters for onsets and codas
static Segment seg_c_p( "p", c_p);
static Segment seg_c_b( "b", c_b);
static Segment seg_c_t( "t", c_t);
static Segment seg_c_d( "d", c_d);
static Segment seg_c_k( "k", c_k);
static Segment seg_c_g( "g", c_g);
static Segment seg_c_m( "m", c_m);
static Segment seg_c_n( "n", c_n);
static Segment seg_c_ng( "ng", c_ng);
static Segment seg_c_f( "f", c_f);
static Segment seg_c_v( "v", c_v);
static Segment seg_c_th0( "th", c_th0);
//static Segment seg_c_th1( "th", c_th1);
static Segment seg_c_s( "s", c_s);
static Segment seg_c_z( "z", c_z);
static Segment seg_c_sh( "sh", c_sh);
static Segment seg_c_zh( "s", c_zh);
static Segment seg_c_h( "h", c_h);
static Segment seg_c_ch( "ch", c_ch);
static Segment seg_c_ge( "j", c_dj);
static Segment seg_c_r( "r", c_r);
static Segment seg_c_j( "y", c_j);
static Segment seg_c_l( "l", c_l);
static Segment seg_plosive_plus_approx_0( "pl",  c_p, c_l );
static Segment seg_plosive_plus_approx_1( "bl",  c_b, c_l );
static Segment seg_plosive_plus_approx_2( "cl",  c_k, c_l );
static Segment seg_plosive_plus_approx_3( "gl",  c_g, c_l );
static Segment seg_plosive_plus_approx_4( "pr",  c_p, c_r );
static Segment seg_plosive_plus_approx_5( "br",  c_b, c_r );
static Segment seg_plosive_plus_approx_6( "tr",  c_t, c_r );
static Segment seg_plosive_plus_approx_7( "dr",  c_d, c_r );
static Segment seg_plosive_plus_approx_8( "cr",  c_k, c_r );
static Segment seg_plosive_plus_approx_9( "gr",  c_g, c_r );
static Segment seg_plosive_plus_approx_10( "tw",  c_t, c_w );
static Segment seg_plosive_plus_approx_11( "dw",  c_d, c_w );
static Segment seg_plosive_plus_approx_12( "gh",  c_g, c_w );		// only for onsets!!!
static Segment seg_plosive_plus_approx_13( "k",  c_k, c_w );
static Segment seg_voiceless_fricative_plus_approx_0( "fl",  c_f, c_l );
static Segment seg_voiceless_fricative_plus_approx_1( "sl",  c_s, c_l );
static Segment seg_voiceless_fricative_plus_approx_2( "fr",  c_f, c_t );
static Segment seg_voiceless_fricative_plus_approx_3( "thr",  c_th0, c_r );
static Segment seg_voiceless_fricative_plus_approx_4( "shr",  c_sh, c_r );
static Segment seg_voiceless_fricative_plus_approx_5( "sw",  c_s, c_w );
static Segment seg_voiceless_fricative_plus_approx_6( "thw",  c_th0, c_w );
static Segment seg_consonant_plus_j_0( "p",  c_p, c_j );
static Segment seg_consonant_plus_j_1( "b",  c_b, c_j );
static Segment seg_consonant_plus_j_2( "t",  c_t, c_j );
static Segment seg_consonant_plus_j_3( "d",  c_d, c_j );
static Segment seg_consonant_plus_j_4( "k",  c_k, c_j );
static Segment seg_consonant_plus_j_5( "g",  c_g, c_j );
static Segment seg_consonant_plus_j_6( "m",  c_m, c_j );
static Segment seg_consonant_plus_j_7( "n",  c_n, c_j );
static Segment seg_consonant_plus_j_8( "f",  c_f, c_j );
static Segment seg_consonant_plus_j_9( "v",  c_v, c_j );
static Segment seg_consonant_plus_j_10( "th",  c_th0, c_j );
static Segment seg_consonant_plus_j_11( "s",  c_s, c_j );
static Segment seg_consonant_plus_j_12( "z",  c_z, c_j );
static Segment seg_consonant_plus_j_13( "h",  c_h, c_j );
static Segment seg_consonant_plus_j_14( "l",  c_l, c_j );
static Segment seg_s_plus_voiceless_plosive_plus_approx_0( "spl",  c_s, c_p, c_l );
static Segment seg_s_plus_voiceless_plosive_plus_approx_1( "spr",  c_s, c_p, c_r );
static Segment seg_s_plus_voiceless_plosive_plus_approx_2( "sp",  c_s, c_p, c_j );
static Segment seg_s_plus_voiceless_plosive_plus_approx_3( "sm",  c_s, c_m, c_j );
static Segment seg_s_plus_voiceless_plosive_plus_approx_4( "str",  c_s, c_t, c_r );
static Segment seg_s_plus_voiceless_plosive_plus_approx_5( "st",  c_s, c_t, c_j );
static Segment seg_s_plus_voiceless_plosive_plus_approx_6( "skl",  c_s, c_k, c_l );
static Segment seg_s_plus_voiceless_plosive_plus_approx_7( "skr",  c_s, c_k, c_r );
static Segment seg_s_plus_voiceless_plosive_plus_approx_8( "sk",  c_s, c_k, c_w );
static Segment seg_s_plus_voiceless_plosive_plus_approx_9( "sk",  c_s, c_k, c_j );
static Segment seg_s_plus_voiceless_plosive_0( "sp",  c_s, c_p );
static Segment seg_s_plus_voiceless_plosive_1( "st",  c_s, c_t );
static Segment seg_s_plus_voiceless_plosive_2( "sk",  c_s, c_k );
static Segment seg_s_plus_nasal_0( "sm",  c_s, c_m );
static Segment seg_s_plus_nasal_1( "sn",  c_s, c_n );
static Segment seg_s_plus_voiceless_fricative_0( "sf",  c_s, c_f );
static Segment seg_lateral_plus_plosive_0( "lp",  c_l, c_p );
static Segment seg_lateral_plus_plosive_1( "lb",  c_l, c_b );
static Segment seg_lateral_plus_plosive_2( "lt",  c_l, c_t );
static Segment seg_lateral_plus_plosive_3( "ld",  c_l, c_d );
static Segment seg_lateral_plus_plosive_4( "lk",  c_l, c_k );
static Segment seg_lateral_plus_fricative_0( "lf",  c_l, c_f );
static Segment seg_lateral_plus_fricative_1( "lv",  c_l, c_v );
static Segment seg_lateral_plus_fricative_2( "lth",  c_l, c_th0 );
static Segment seg_lateral_plus_fricative_3( "ls",  c_l, c_s );
static Segment seg_lateral_plus_fricative_4( "lsh",  c_l, c_sh );
static Segment seg_lateral_plus_affricate_0( "lch",  c_l, c_ch );
static Segment seg_lateral_plus_affricate_1( "lj",  c_l, c_dj );
static Segment seg_lateral_plus_nasal_0( "lm",  c_l, c_m );
static Segment seg_lateral_plus_nasal_1( "ln",  c_l, c_n );
static Segment seg_nasal_plus_plosive_0( "mp",  c_m, c_p );
static Segment seg_nasal_plus_plosive_1( "nt",  c_n, c_t );
static Segment seg_nasal_plus_plosive_2( "nd",  c_n, c_d );
static Segment seg_nasal_plus_plosive_3( "nk",  c_ng, c_k );
static Segment seg_nasal_plus_fricative_0( "mf",  c_m, c_f );
static Segment seg_nasal_plus_fricative_1( "mth",  c_m, c_th0 );
static Segment seg_nasal_plus_fricative_2( "nth",  c_n, c_th0 );
static Segment seg_nasal_plus_fricative_3( "ns",  c_n, c_s );
static Segment seg_nasal_plus_fricative_4( "nz",  c_n, c_z );
static Segment seg_nasal_plus_fricative_5( "ngth",  c_ng, c_th0 );
static Segment seg_nasal_plus_affricate_0( "nch",  c_n, c_ch );
static Segment seg_nasal_plus_affricate_1( "nj",  c_n, c_dj );
static Segment seg_voiceless_fricative_plus_voiceless_plosive_0( "ft",  c_f, c_t );
static Segment seg_voiceless_fricative_plus_voiceless_plosive_1( "sp",  c_s, c_p );
static Segment seg_voiceless_fricative_plus_voiceless_plosive_2( "st",  c_s, c_t );
static Segment seg_voiceless_fricative_plus_voiceless_plosive_3( "sk",  c_s, c_k );
static Segment seg_voiceless_fricative_plus_voiceless_fricative_0( "fth",  c_f, c_th0 );
static Segment seg_voiceless_plosive_plus_voiceless_plosive_0( "pt",  c_p, c_t );
static Segment seg_voiceless_plosive_plus_voiceless_plosive_1( "ct",  c_k, c_t );
static Segment seg_plosive_plus_voiceless_fricative_0( "pth",  c_p, c_th0 );
static Segment seg_plosive_plus_voiceless_fricative_1( "ps",  c_p, c_s );
static Segment seg_plosive_plus_voiceless_fricative_2( "tth",  c_t, c_th0 );
static Segment seg_plosive_plus_voiceless_fricative_3( "ts",  c_t, c_s );
static Segment seg_plosive_plus_voiceless_fricative_4( "dth",  c_d, c_th0 );
static Segment seg_plosive_plus_voiceless_fricative_5( "dz",  c_d, c_z );
static Segment seg_plosive_plus_voiceless_fricative_6( "x",  c_k, c_s );
static Segment seg_lateral_plus_two_consonants_0( "lpt",  c_l, c_p, c_t );
static Segment seg_lateral_plus_two_consonants_1( "lfth",  c_l, c_f, c_th0 );
static Segment seg_lateral_plus_two_consonants_2( "lts",  c_l, c_t, c_s );
static Segment seg_lateral_plus_two_consonants_3( "lst",  c_l, c_s, c_t );
static Segment seg_lateral_plus_two_consonants_4( "lct",  c_l, c_k, c_t );
static Segment seg_lateral_plus_two_consonants_5( "lx",  c_l, c_k, c_s );
static Segment seg_nasal_plus_two_plosives_0( "mpt",  c_m, c_p, c_t );
static Segment seg_nasal_plus_two_plosives_1( "mps",  c_m, c_p, c_s );
static Segment seg_nasal_plus_two_plosives_2( "nkt",  c_ng, c_k, c_t );
static Segment seg_nasal_plus_two_plosives_3( "nx",  c_ng, c_k, c_s );
static Segment seg_nasal_plus_plosive_plus_fricative_0( "ndth",  c_n, c_d, c_th0 );
static Segment seg_nasal_plus_plosive_plus_fricative_1( "ngth",  c_n, c_g, c_th0 );
static Segment seg_three_obstruent_0( "xth",  c_k, c_s, c_th0 );
static Segment seg_three_obstruent_1( "xt",  c_k, c_s, c_t );


static Segment seg_null;

void en_setupOnsets(SegmentDistribution &dist) {

	dist.addItem( 30, seg_null);
	dist.addItem( 30, seg_c_p );
	dist.addItem( 30, seg_c_b );
	dist.addItem( 30, seg_c_t );
	dist.addItem( 30, seg_c_d );
	dist.addItem( 30, seg_c_k );
	dist.addItem( 30, seg_c_g );
	dist.addItem( 30, seg_c_m );
	dist.addItem( 30, seg_c_n );
	dist.addItem( 30, seg_c_f );
	dist.addItem( 30, seg_c_v );
	dist.addItem( 30, seg_c_th0 );
//	dist.addItem( 30, seg_c_th1 );
	dist.addItem( 30, seg_c_s );
	dist.addItem( 30, seg_c_z );
	dist.addItem( 30, seg_c_sh );
	dist.addItem( 30, seg_c_zh );
	dist.addItem( 30, seg_c_h );
	dist.addItem( 30, seg_c_ch );
	dist.addItem( 30, seg_c_ge );
	dist.addItem( 30, seg_c_r );
	dist.addItem( 30, seg_c_j );
	dist.addItem( 30, seg_c_l );
	dist.addItem( 1, seg_plosive_plus_approx_0 );
	dist.addItem( 1, seg_plosive_plus_approx_1 );
	dist.addItem( 1, seg_plosive_plus_approx_2 );
	dist.addItem( 1, seg_plosive_plus_approx_3 );
	dist.addItem( 1, seg_plosive_plus_approx_4 );
	dist.addItem( 1, seg_plosive_plus_approx_5 );
	dist.addItem( 1, seg_plosive_plus_approx_6 );
	dist.addItem( 1, seg_plosive_plus_approx_7 );
	dist.addItem( 1, seg_plosive_plus_approx_8 );
	dist.addItem( 1, seg_plosive_plus_approx_9 );
	dist.addItem( 1, seg_plosive_plus_approx_10 );
	dist.addItem( 1, seg_plosive_plus_approx_11 );
	dist.addItem( 1, seg_plosive_plus_approx_12 );
	dist.addItem( 1, seg_plosive_plus_approx_13 );
	dist.addItem( 1, seg_voiceless_fricative_plus_approx_0 );
	dist.addItem( 1, seg_voiceless_fricative_plus_approx_1 );
	dist.addItem( 1, seg_voiceless_fricative_plus_approx_2 );
	dist.addItem( 1, seg_voiceless_fricative_plus_approx_3 );
	dist.addItem( 1, seg_voiceless_fricative_plus_approx_4 );
	dist.addItem( 1, seg_voiceless_fricative_plus_approx_5 );
	dist.addItem( 1, seg_voiceless_fricative_plus_approx_6 );
	dist.addItem( 1, seg_consonant_plus_j_0 );
	dist.addItem( 1, seg_consonant_plus_j_1 );
	dist.addItem( 1, seg_consonant_plus_j_2 );
	dist.addItem( 1, seg_consonant_plus_j_3 );
	dist.addItem( 1, seg_consonant_plus_j_4 );
	dist.addItem( 1, seg_consonant_plus_j_5 );
	dist.addItem( 1, seg_consonant_plus_j_6 );
	dist.addItem( 1, seg_consonant_plus_j_7 );
	dist.addItem( 1, seg_consonant_plus_j_8 );
	dist.addItem( 1, seg_consonant_plus_j_9 );
	dist.addItem( 1, seg_consonant_plus_j_10 );
	dist.addItem( 1, seg_consonant_plus_j_11 );
	dist.addItem( 1, seg_consonant_plus_j_12 );
	dist.addItem( 1, seg_consonant_plus_j_13 );
	dist.addItem( 1, seg_consonant_plus_j_14 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_0 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_1 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_2 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_3 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_4 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_5 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_6 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_7 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_8 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_plus_approx_9 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_0 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_1 );
	dist.addItem( 1, seg_s_plus_voiceless_plosive_2 );
	dist.addItem( 1, seg_s_plus_nasal_0 );
	dist.addItem( 1, seg_s_plus_nasal_1 );
	dist.addItem( 1, seg_s_plus_voiceless_fricative_0 );

}

void en_setupCodas(SegmentDistribution &dist) {

	dist.addItem( 15, seg_null);
	dist.addItem( 30, seg_c_p );
	dist.addItem( 30, seg_c_b );
	dist.addItem( 30, seg_c_t );
	dist.addItem( 30, seg_c_d );
	dist.addItem( 30, seg_c_k );
	dist.addItem( 30, seg_c_g );
	dist.addItem( 30, seg_c_m );
	dist.addItem( 30, seg_c_n );
	dist.addItem( 1, seg_c_ng );
	dist.addItem( 30, seg_c_f );
	dist.addItem( 30, seg_c_v );
	dist.addItem( 30, seg_c_th0 );
//	dist.addItem( 30, seg_c_th1 );
	dist.addItem( 30, seg_c_s );
	dist.addItem( 30, seg_c_z );
	dist.addItem( 1, seg_c_sh );
	dist.addItem( 1, seg_c_zh );
	dist.addItem( 1, seg_c_ch );
	dist.addItem( 1, seg_c_ge );
	dist.addItem( 30, seg_c_r );
	dist.addItem( 10, seg_c_l );
	dist.addItem( 1, seg_lateral_plus_plosive_0 );
	dist.addItem( 1, seg_lateral_plus_plosive_1 );
	dist.addItem( 1, seg_lateral_plus_plosive_2 );
	dist.addItem( 1, seg_lateral_plus_plosive_3 );
	dist.addItem( 1, seg_lateral_plus_plosive_4 );
	dist.addItem( 1, seg_lateral_plus_fricative_0 );
	dist.addItem( 1, seg_lateral_plus_fricative_1 );
	dist.addItem( 1, seg_lateral_plus_fricative_2 );
	dist.addItem( 1, seg_lateral_plus_fricative_3 );
	dist.addItem( 1, seg_lateral_plus_fricative_4 );
	dist.addItem( 1, seg_lateral_plus_affricate_0 );
	dist.addItem( 1, seg_lateral_plus_affricate_1 );
	dist.addItem( 1, seg_lateral_plus_nasal_0 );
	dist.addItem( 1, seg_lateral_plus_nasal_1 );
	dist.addItem( 1, seg_nasal_plus_plosive_0 );
	dist.addItem( 1, seg_nasal_plus_plosive_1 );
	dist.addItem( 1, seg_nasal_plus_plosive_2 );
	dist.addItem( 1, seg_nasal_plus_plosive_3 );
	dist.addItem( 1, seg_nasal_plus_fricative_0 );
	dist.addItem( 1, seg_nasal_plus_fricative_1 );
	dist.addItem( 1, seg_nasal_plus_fricative_2 );
	dist.addItem( 1, seg_nasal_plus_fricative_3 );
	dist.addItem( 1, seg_nasal_plus_fricative_4 );
	dist.addItem( 1, seg_nasal_plus_fricative_5 );
	dist.addItem( 1, seg_nasal_plus_affricate_0 );
	dist.addItem( 1, seg_nasal_plus_affricate_1 );
	dist.addItem( 1, seg_voiceless_fricative_plus_voiceless_plosive_0 );
	dist.addItem( 1, seg_voiceless_fricative_plus_voiceless_plosive_1 );
	dist.addItem( 1, seg_voiceless_fricative_plus_voiceless_plosive_2 );
	dist.addItem( 1, seg_voiceless_fricative_plus_voiceless_plosive_3 );
	dist.addItem( 0, seg_voiceless_fricative_plus_voiceless_fricative_0 );
	dist.addItem( 1, seg_voiceless_plosive_plus_voiceless_plosive_0 );
	dist.addItem( 1, seg_voiceless_plosive_plus_voiceless_plosive_1 );
	dist.addItem( 0, seg_plosive_plus_voiceless_fricative_0 );
	dist.addItem( 0, seg_plosive_plus_voiceless_fricative_1 );
	dist.addItem( 0, seg_plosive_plus_voiceless_fricative_2 );
	dist.addItem( 0, seg_plosive_plus_voiceless_fricative_3 );
	dist.addItem( 0, seg_plosive_plus_voiceless_fricative_4 );
	dist.addItem( 0, seg_plosive_plus_voiceless_fricative_5 );
	dist.addItem( 0, seg_plosive_plus_voiceless_fricative_6 );
	dist.addItem( 0, seg_lateral_plus_two_consonants_0 );
	dist.addItem( 0, seg_lateral_plus_two_consonants_1 );
	dist.addItem( 0, seg_lateral_plus_two_consonants_2 );
	dist.addItem( 0, seg_lateral_plus_two_consonants_3 );
	dist.addItem( 0, seg_lateral_plus_two_consonants_4 );
	dist.addItem( 0, seg_lateral_plus_two_consonants_5 );
	dist.addItem( 0, seg_nasal_plus_two_plosives_0 );
	dist.addItem( 0, seg_nasal_plus_two_plosives_1 );
	dist.addItem( 0, seg_nasal_plus_two_plosives_2 );
	dist.addItem( 0, seg_nasal_plus_two_plosives_3 );
	dist.addItem( 0, seg_nasal_plus_plosive_plus_fricative_0 );
	dist.addItem( 0, seg_nasal_plus_plosive_plus_fricative_1 );
	dist.addItem( 0, seg_three_obstruent_0 );
	dist.addItem( 0, seg_three_obstruent_1 );

}

void en_setupNuclei(SegmentDistribution &dist) {

	dist.addItem( 10, seg_sv_i );
	dist.addItem( 10, seg_sv_u );
	dist.addItem( 10, seg_sv_e0 );
	dist.addItem( 10, seg_sv_e1 );
	dist.addItem( 10, seg_sv_a0 );
	dist.addItem( 10, seg_sv_a1 );
	dist.addItem( 10, seg_sv_o );
	dist.addItem( 10, seg_lv_i );
	dist.addItem( 10, seg_lv_u );
	dist.addItem( 10, seg_lv_e );
	dist.addItem( 10, seg_lv_o );
	dist.addItem( 10, seg_lv_a );
	dist.addItem( 1, seg_diph_ei );
	dist.addItem( 1, seg_diph_ou );
	dist.addItem( 1, seg_diph_ai );
	dist.addItem( 1, seg_diph_au );
	dist.addItem( 1, seg_diph_oi );
	dist.addItem( 1, seg_diph_uschwa );
	dist.addItem( 1, seg_diph_eschwa );

}
