#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

// Get pixel per point scale for current display
extern "C" float getPixelPointScale() {
    NSScreen *mainScreen = [NSScreen mainScreen];
    CGFloat scale = mainScreen.backingScaleFactor;
    return (float)scale;
}
