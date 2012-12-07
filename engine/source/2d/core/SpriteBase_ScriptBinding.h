//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, isStaticMode, bool, 2, 2,     "() - Gets whether the sprite is in static or dynamic (animated)mode.\n"
                                                    "@return Returns whether the sprite is in static or dynamic (animated)mode.")
{
    return object->isStaticMode();
}

//------------------------------------------------------------------------------

ConsoleMethod(SpriteBase, setImageMap, bool, 3, 4,  "(string imageAssetId, [int frame]) - Sets the sprite image and optionally frame.\n"
                                                    "@param imageAssetId The image asset Id to display\n"
                                                    "@param frame The frame of the image to display\n"
                                                    "@return Returns true on success.")
{
    // Calculate Frame.
    U32 frame = argc >= 4 ? dAtoi(argv[3]) : 0;

    // Set image.
    return static_cast<SpriteProxyBase*>(object)->setImage( argv[2], frame );
}

//------------------------------------------------------------------------------

ConsoleMethod(SpriteBase, getImageMap, const char*, 2, 2,   "() - Gets current image asset Id.\n"
                                                        "@return (string imageAssetId) The image being displayed")
{
    // Are we in static mode?
    if ( !object->isStaticMode() )
    {
        // No, so warn.
        Con::warnf( "SpriteBase::getImageMap() - Method invalid, not in static mode." );
        return StringTable->EmptyString;
    }

    // Get image.
    return static_cast<SpriteProxyBase*>(object)->getImage();
}   

//------------------------------------------------------------------------------

ConsoleMethod(SpriteBase, setFrame, bool, 3, 3,     "(int frame) - Sets image frame.\n"
                                                "@param frame The frame to display\n"
                                                "@return Returns true on success.")
{
    // Are we in static mode?
    if ( !object->isStaticMode() )
    {
        // No, so warn.
        Con::warnf( "SpriteBase::setFrame() - Method invalid, not in static mode." );
        return false;
    }

    // Set image Frame.
    return static_cast<SpriteProxyBase*>(object)->setImageFrame( dAtoi(argv[2]) );
}

//------------------------------------------------------------------------------

ConsoleMethod(SpriteBase, getFrame, S32, 2, 2,  "() - Gets current image Frame.\n"
                                            "@return (int frame) The frame currently being displayed")
{
    // Are we in static mode?
    if ( !object->isStaticMode() )
    {
        // No, so warn.
        Con::warnf( "SpriteBase::getFrame() - Method invalid, not in static mode." );
        return -1;
    }

    // Get image Frame.
    return static_cast<SpriteProxyBase*>(object)->getImageFrame();
}

//------------------------------------------------------------------------------

ConsoleMethod(SpriteBase, playAnimation, bool, 3, 4,    "(string animationAssetId, [bool autoRestore]) - Plays an animation.\n"
                                                    "@param animationAssetId The animation asset Id to play.\n"
                                                    "@param autoRestore If true, the previous animation will be played when this new animation finishes.\n"
                                                    "@return Returns true on success.")
{    
    // Fetch Auto-Restore Flag.
    const bool autoRestore = (argc >= 4) ? dAtob(argv[3]) : false;

    // Play Animation.
    return static_cast<SpriteProxyBase*>(object)->setAnimation( argv[2], autoRestore );
}   

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, pauseAnimation, void, 3, 3, "(bool enable) - Pause the current animation\n"
                                                             "@param enable If true, pause the animation. If false, continue animating\n")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::pauseAnimation() - Method invalid, not in dynamic (animated) mode." );
        return;
    }

    return static_cast<SpriteProxyBase*>(object)->pauseAnimation(dAtob(argv[2]));
}


//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, stopAnimation, void, 2, 2,   "() - Stop the current animation\n"
                                                        "@return No return value.")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::stopAnimation() - Method invalid, not in dynamic (animated) mode." );
        return;
    }

    object->getAnimationController()->stopAnimation();
}

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, setAnimationFrame, void, 3, 3, "(int frame) - Sets the current animation frame.\n"
                                                                "@param frame Which frame of the animation to display\n"
                                                                "@return No return value.")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::setAnimationFrame() - Method invalid, not in dynamic (animated) mode." );
        return;
    }

    // Set Animation Frame
    object->getAnimationController()->setAnimationFrame( dAtoi(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, getAnimationFrame, S32, 2, 2, "() - Gets current animation frame.\n"
                                                               "@return (int frame) The current animation frame")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::getAnimationFrame() - Method invalid, not in dynamic (animated) mode." );
        return -1;
    }

    // Get Animation Frame.
    return object->getAnimationController()->getCurrentFrame();
}

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, getAnimation, const char*, 2, 2,  "() - Gets current animation asset Id.\n"
                                                        "@return (string AnimationAssetId) The current animation asset Id.")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::getAnimation() - Method invalid, not in dynamic (animated) mode." );
        return StringTable->EmptyString;
    }


    // Get Current Animation.
    return object->getAnimationController()->getCurrentAnimation();
}

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, getAnimationTime, F32, 2, 2,  "() - Gets current animation time.\n"
                                                    "@return (float time) The current animation time")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::getAnimationTime() - Method invalid, not in dynamic (animated) mode." );
        return 0.0f;
    }


    // Get Animation Time.
    return object->getAnimationController()->getCurrentTime();
}

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, getIsAnimationFinished, bool, 2, 2,   "() - Checks animation status.\n"
                                                            "@return (bool finished) Whether or not the animation is finished")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::getIsAnimationFinished() - Method invalid, not in dynamic (animated) mode." );
        return true;
    }

    // Return Animation Finished Status.
    return object->getAnimationController()->isAnimationFinished();
}

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, setAnimationTimeScale, void, 3, 3,   "(float timeScale) - Change the rate of animation.\n"
                                                            "@param timeScale Value which will scale the frame animation speed. 1 by default.\n")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::setAnimationTimeScale() - Method invalid, not in dynamic (animated) mode." );
        return;
    }

    object->getAnimationController()->setAnimationTimeScale(dAtof(argv[2]));
}

//-----------------------------------------------------------------------------

ConsoleMethod(SpriteBase, getAnimationTimeScale, F32, 2, 2,     "() - Get the animation time scale for this sprite.\n"
                                                            "@return (float) Returns the animation time scale for this sprite.\n")
{
    // Are we in static mode?
    if ( object->isStaticMode() )
    {
        // Yes, so warn.
        Con::warnf( "SpriteBase::getSpeedFactor() - Method invalid, not in dynamic (animated) mode." );
        return 1.0f;
    }

    return object->getAnimationController()->getAnimationTimeScale();
}
