/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

void GL_BeginRendering (int *x, int *y, int *width, int *height);
void GL_EndRendering (void);

extern	float	gldepthmin, gldepthmax;

typedef struct
{
	float	x, y, z;
	float	s, t;
	float	r, g, b;
} glvert_t;

extern glvert_t glv;

extern	int glx, gly, glwidth, glheight;

// r_local.h -- private refresh defs

#define ALIAS_BASE_SIZE_RATIO		(1.0 / 11.0)
					// normalizing factor so player model works out to about
					//  1 pixel per triangle
#define	MAX_LBM_HEIGHT		480

#define TILE_SIZE		128		// size of textures generated by R_GenTiledSurf

#define SKYSHIFT		7
#define	SKYSIZE			(1 << SKYSHIFT)
#define SKYMASK			(SKYSIZE - 1)

#define BACKFACE_EPSILON	0.01


void R_TimeRefresh_f (void);
void R_ReadPointFile_f (void);
texture_t *R_TextureAnimation (texture_t *base);

typedef struct surfcache_s
{
	struct surfcache_s	*next;
	struct surfcache_s 	**owner;		// NULL is an empty chunk of memory
	int					lightadj[MAXLIGHTMAPS]; // checked for strobe flush
	int					dlight;
	int					size;		// including header
	unsigned			width;
	unsigned			height;		// DEBUG only needed for debug
	float				mipscale;
	struct texture_s	*texture;	// checked for animating textures
	byte				data[4];	// width*height elements
} surfcache_t;


typedef struct
{
	pixel_t		*surfdat;	// destination for generated surface
	int			rowbytes;	// destination logical width in bytes
	msurface_t	*surf;		// description for surface to generate
	fixed8_t	lightadj[MAXLIGHTMAPS];
							// adjust for lightmap levels for dynamic lighting
	texture_t	*texture;	// corrected for animating textures
	int			surfmip;	// mipmapped ratio of surface texels / world pixels
	int			surfwidth;	// in mipmapped texels
	int			surfheight;	// in mipmapped texels
} drawsurf_t;


typedef enum {
	pt_static, pt_grav, pt_slowgrav, pt_fire, pt_explode, pt_explode2, pt_blob, pt_blob2
} ptype_t;

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
typedef struct particle_s
{
// driver-usable fields
	vec3_t		org;
	float		color;
// drivers never touch the following fields
	struct particle_s	*next;
	vec3_t		vel;
	float		ramp;
	float		die;
	ptype_t		type;
} particle_t;


//====================================================


extern	entity_t	r_worldentity;
extern	qboolean	r_cache_thrash;		// compatability
extern	vec3_t		modelorg, r_entorigin;
extern	entity_t	*currententity;
extern	int			r_visframecount;	// ??? what difs?
extern	int			r_framecount;
extern	mplane_t	frustum[4];
extern	int		c_brush_polys, c_alias_polys;


//
// view origin
//
extern	vec3_t	vup;
extern	vec3_t	vpn;
extern	vec3_t	vright;
extern	vec3_t	r_origin;

//
// screen size info
//
extern	refdef_t	r_refdef;
extern	mleaf_t		*r_viewleaf, *r_oldviewleaf;
extern	texture_t	*r_notexture_mip;
extern	int		d_lightstylevalue[256];	// 8.8 fraction of base light value

extern	qboolean	envmap;
extern	int	currenttexture0;
extern	int	currenttexture1;
extern	int	cnttextures[2];
extern	int	particletexture;
extern	int	playertextures;

extern	int	skytexturenum;		// index in cl.loadmodel, not gl texture object

extern	cvar_t	r_norefresh;
extern	cvar_t	r_drawentities;
extern	cvar_t	r_drawworld;
extern	cvar_t	r_drawviewmodel;
extern	cvar_t	r_speeds;
extern	cvar_t	r_waterwarp;
extern	cvar_t	r_fullbright;
extern	cvar_t	r_lightmap;
extern	cvar_t	r_shadows;
extern	cvar_t	r_mirroralpha;
extern	cvar_t	r_wateralpha;
extern	cvar_t	r_dynamic;
extern	cvar_t	r_novis;

extern	cvar_t	gl_clear;
extern	cvar_t	gl_cull;
extern	cvar_t	gl_poly;
extern	cvar_t	gl_smoothmodels;
extern	cvar_t	gl_affinemodels;
extern	cvar_t	gl_polyblend;
extern	cvar_t	gl_keeptjunctions;
extern	cvar_t	gl_reporttjunctions;
extern	cvar_t	gl_nocolors;
extern	cvar_t	gl_doubleeyes;

extern	cvar_t	gl_max_size;
extern	cvar_t	gl_playermip;

extern int white_texturenum;

extern	int			mirrortexturenum;
extern	qboolean	mirror;
extern	mplane_t	*mirror_plane;

extern	float	r_world_matrix[16];

void GL_Bind0 (int texnum);
void GL_Bind1 (int texnum);

extern vrect_t scr_vrect;

extern Mtx44 perspective;
extern Mtx view, model, modelview;

#define ZMIN3D			4.0f
#define ZMAX3D			16384.0f
#define ZMIN2D			-9999.0f
#define ZMAX2D			9999.0f

// Textures

typedef struct
{
	int			texnum;
	GXTexObj	gx_tex;
	char		identifier[64];
	int			width, height;
	qboolean	mipmap;
	unsigned	*data;
	int			scaled_width, scaled_height;

	// ELUTODO: make sure textures loaded without an identifier are loaded only one time, if "keep" is on
	// What about mid-game gamma changes?
	qboolean	type; // 0 = normal, 1 = lightmap
	qboolean	keep;

	qboolean	used;
} gltexture_t;

#define	MAX_GLTEXTURES	1024
extern int numgltextures;
extern gltexture_t	gltextures[MAX_GLTEXTURES];

void QGX_ZMode(qboolean state);
void QGX_Alpha(qboolean state);
void QGX_Blend(qboolean state);

int GL_LoadTexture (char *identifier, int width, int height, byte *data, qboolean mipmap, qboolean alpha, qboolean keep);
int GL_LoadLightmapTexture (char *identifier, int width, int height, byte *data);
void GL_UpdateTexture (int pic_id, char *identifier, int width, int height, byte *data, qboolean mipmap, qboolean alpha);
void GL_UpdateLightmapTextureRegion (int pic_id, int width, int height, int xoffset, int yoffset, byte *data);
int GL_FindTexture (char *identifier);

void GL_DisableMultitexture(void);
void GL_EnableMultitexture(void);

void GL_ClearTextureCache(void);

// OSK
void GX_DrawOSK(void);
extern int in_osk;
extern char *osk_set;
extern int osk_selected;
extern int osk_coords[2];
#define osk_charsize 8
// ELUTODO: use glwidth/height or wiimote_ir_res_x/y?
#define osk_num_lines 5
#define osk_line_size 16
#define osk_num_col 15
#define osk_col_size 16
#define OSK_XSTART (((glwidth) - ((glwidth) / (osk_col_size) * (osk_num_col))) / 2)
#define OSK_YSTART (((glheight) - ((glheight) / (osk_line_size) * (osk_num_lines))) / 2)

// Draw.c extensions
extern void Draw_TransAlphaPic (int x, int y, qpic_t *pic, float alpha);
extern void Draw_AlphaPic (int x, int y, qpic_t *pic, float alpha);
extern void Draw_AlphaTileClear (int x, int y, int w, int h, float alpha);
