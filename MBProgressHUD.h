//
//  MBProgressHUD.h
//  Created by Matej Bukovinski on 2.4.09.
//
// This code is licensed under MIT. See LICENSE for more information. 
//

#import <UIKit/UIKit.h>

/** When set as the HUD's custom view, the HUD will show a check image. **/
extern const id MBProgressHUDSuccessImageView;

/** When set as the HUD's custom view, the HUD will show an error image. **/
extern const id MBProgressHUDErrorImageView;

typedef enum {
    /** Progress is shown using an UIActivityIndicatorView. This is the default. */
    MBProgressHUDModeIndeterminate,
    /** Progress is shown using a MBRoundProgressView. */
	MBProgressHUDModeDeterminate,
	/** Shows a custom view */
	MBProgressHUDModeCustomView
} MBProgressHUDMode;

/** 
 * Displays a simple HUD window containing a progress indicator and two optional labels for short messages.
 *
 * This is a simple drop-in class for displaying a progress HUD view similar to the
 * private UIProgressHUD. The HUD view spans over the entire space given to it by
 * its superview and catches all touchs on that region, thereby preventing user
 * interaction on components below the view. The HUD itself is drawn centered as
 * a rounded semi-transparent box that resizes depending on its content.
 *
 * The HUD supports three modes of operation:
 * - MBProgressHUDModeIndeterminate - shows a UIActivityIndicatorView
 * - MBProgressHUDModeDeterminate - shows a custom round progress indicator
 * - MBProgressHUDModeCustomView - shows an arbitrary, user specified view
 *
 * All three modes have an optional label. If the label has text, then the label
 * is placed below the indicator view.
 *
 * @see customView
 */
@interface MBProgressHUD : UIView

/**
 * Creates a new HUD, adds it to provided view and shows it.
 * 
 * @param view The view that the HUD will be added to
 * @param animated If set to YES, the HUD will disappear with an animated transition.
 * @return A reference to the created HUD.
 *
 * @see hideHUDForView:animated:
 */
+ (MBProgressHUD *)showHUDAddedTo:(UIView *)view animated:(BOOL)animated;

/**
 * Finds a HUD subview and hides it.
 *
 * @param view The view that is going to be searched for a HUD subview.
 * @param animated If set to YES, the HUD will disappear with an animated transition.
 * @return YES if a HUD was found and removed, NO otherwise. 
 *
 * @see showHUDAddedTo:animated:
 */
+ (BOOL)hideHUDForView:(UIView *)view animated:(BOOL)animated;

/**
 * Shows a HUD on the current window using while executing a block in the background.
 *
 * The block is executed from a separate queue unrelated to the UI main thread. The HUD is passed
 * as an argument to the block for progress updates, changes, and so on. The HUD should not be
 * dismissed from within the block; instead, bail out using a return statement.
 *
 * @param statusText The text for the main label. Send an empty string to not show the label at all.
 * @param block A code block to be executed. Should not be NULL.
 *
 * @see label
 * @see showWhileExecuting:
 */
+ (void)showWithText:(NSString *)statusText whileExecuting:(void(^)(MBProgressHUD *))block;

/**
 * The view to be shown when the HUD is set to MBProgressHUDModeCustomView.
 * For best results, use a 37x37 view (so the bounds match the default indicator bounds). 
 *
 * Pass `MBProgressHUDSuccessImageView` for an image view with a check.
 * Pass `MBProgressHUDErrorImageView`, for an image view with an error symbol.
 **/
@property (nonatomic, strong) UIView *customView;

/** 
 * HUD operation mode. The default is MBProgressHUDModeIndeterminate.
 *
 * @see MBProgressHUDMode
 */
@property (nonatomic) MBProgressHUDMode mode;

/** A callback fired when the HUD is tapped. */
@property (nonatomic, copy) void(^wasTappedBlock)(MBProgressHUD *);

/** A callback fired when the HUD is hidden. */
@property (nonatomic, copy) void(^wasHiddenBlock)(MBProgressHUD *);

/*
 * The show delay is the time (in seconds) that your method may run without the HUD
 * being shown. If the task finishes before the grace time runs out, the HUD will
 * not appear at all.
 *
 * Defaults to 0. If you don't set one and still might have a short task,
 * it is recommended to set a minimum show time instead.
 *
 * @see minimumShowTime
 */
@property (nonatomic) NSTimeInterval showDelayTime;

/**
 * The minimum time (in seconds) that the HUD is shown. 
 * This avoids the problem of the HUD being shown and than instantly hidden.
 *
 * Defaults to 1.5. If you don't set one and your task might run short,
 * it is recommended to instead set a show delay time.
 *
 * @see showDelayTime
 */
@property (nonatomic) NSTimeInterval minimumShowTime;

/** Removes the HUD from its parent view when hidden. Defaults to NO. */
@property (nonatomic) BOOL removeFromSuperViewOnHide;

/** Returns the label used for the main textual content of the HUD. */
@property (nonatomic, unsafe_unretained, readonly) UILabel *label;

/** 
 * The progress of the progress indicator, from 0.0 to 1.0.
 */
@property (nonatomic) CGFloat progress;

/** 
 * The progress of the progress indicator, from 0.0 to 1.0. Animatable.
 *
 * @param progress A new value for the progress indicator.
 * @param animated If YES, the progress change is animated.
 */
- (void)setProgress:(CGFloat)progress animated:(BOOL)animated;

/** 
 * Display the HUD.
 *
 * @param animated If set to YES, the HUD will disappear with an animated transition.
 * If set to NO the HUD will not use animations while disappearing.
 */
- (void)show:(BOOL)animated;

/** 
 * Hide the HUD. Use it to hide the HUD when your task completes.
 *
 * @param animated If set to YES, the HUD will disappear with an animated transition.
 * @see hide:completion:
 */
- (void)hide:(BOOL)animated;

/** 
 * Hide the HUD with a completion callback. Use it to hide the HUD when your task completes.
 *
 * @param animated If set to YES, the HUD will disappear using the current animation type.
 * @param completion A block to be called once the HUD has disappeared.
 * @see hide:
 * @see wasHiddenBlock
 */
- (void)hide:(BOOL)animated completion:(dispatch_block_t)completion;

/** 
 * Shows the HUD while a task is executing in a background queue, then hides it.
 *
 * This method also takes care of an autorelease pools so your method does not have
 * to be concerned with setting one up.
 *
 * The HUD will appear and disappear using the current animationType.
 *
 * @param block A code block to be executed while the HUD is shown.
 */
- (void)showWhileExecuting:(dispatch_block_t)block;

/**
 * Coalesces changes to the HUD (mode, view, text, fonts) into a single animation.
 *
 * This method is non-blocking, but the HUD cannot be hidden while animations are ongoing.
 *
 * @param A code block of changes to the HUD. Will be executed from within a transition method.
 */
- (void)performChanges:(dispatch_block_t)animations;

@end

#pragma mark -

/** A progress view for showing definite progress by filling up a circle (pie chart). */
@interface MBRoundProgressView : UIView

@property (nonatomic) CGFloat progress;

- (void)setProgress:(CGFloat)progress animated:(BOOL)animated;

@end