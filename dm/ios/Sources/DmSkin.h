// Geometry for the optional Strata-style controller artwork. Image pixel coordinates
// were measured from the two supplied 1792x1008 and 923x2000 skin images.
// Keep the game view INSIDE the transparent cutout, with its aspect ratio intact.
#pragma once
#import <UIKit/UIKit.h>

typedef struct { CGSize pixels; CGRect cutout; } DmSkinSpec;

static inline DmSkinSpec dm_skin_spec(BOOL portrait) {
    if (portrait) return (DmSkinSpec){ CGSizeMake(923, 2000), CGRectMake(37, 138, 847, 661) };
    return (DmSkinSpec){ CGSizeMake(1792, 1008), CGRectMake(463, 169, 863, 574) };
}
static inline CGFloat dm_skin_scale(CGRect bounds, BOOL portrait) {
    DmSkinSpec spec = dm_skin_spec(portrait);
    if (bounds.size.width <= 0 || bounds.size.height <= 0) return 0;
    return MIN(bounds.size.width / spec.pixels.width, bounds.size.height / spec.pixels.height);
}
static inline CGRect dm_skin_art_rect(CGRect bounds, BOOL portrait) {
    DmSkinSpec spec = dm_skin_spec(portrait);
    CGFloat s = dm_skin_scale(bounds, portrait);
    CGSize size = CGSizeMake(spec.pixels.width * s, spec.pixels.height * s);
    return CGRectMake(CGRectGetMidX(bounds) - size.width / 2,
                      CGRectGetMidY(bounds) - size.height / 2,
                      size.width, size.height);
}
static inline CGPoint dm_skin_point(CGRect bounds, BOOL portrait, CGFloat x, CGFloat y) {
    CGRect art = dm_skin_art_rect(bounds, portrait);
    CGFloat s = dm_skin_scale(bounds, portrait);
    return CGPointMake(art.origin.x + x * s, art.origin.y + y * s);
}
static inline CGRect dm_skin_cutout(CGRect bounds, BOOL portrait) {
    CGRect pixelHole = dm_skin_spec(portrait).cutout;
    CGPoint topLeft = dm_skin_point(bounds, portrait, pixelHole.origin.x, pixelHole.origin.y);
    CGFloat s = dm_skin_scale(bounds, portrait);
    return CGRectMake(topLeft.x, topLeft.y, pixelHole.size.width * s, pixelHole.size.height * s);
}
static inline CGRect dm_skin_game_rect(CGRect bounds, BOOL portrait, CGFloat aspect) {
    CGRect hole = dm_skin_cutout(bounds, portrait);
    if (aspect <= 0 || hole.size.width <= 0 || hole.size.height <= 0) return CGRectZero;
    CGFloat width = MIN(hole.size.width, hole.size.height * aspect);
    CGFloat height = width / aspect;
    return CGRectMake(CGRectGetMidX(hole) - width / 2,
                      CGRectGetMidY(hole) - height / 2, width, height);
}
