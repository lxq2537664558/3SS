//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _BEHAVIORCOMPONENT_RAISEEVENT_H_
#include "behaviorComponentRaiseEvent.h"
#endif

//-----------------------------------------------------------------------------

ConsoleFunction( copyBehaviorToComponent, bool, 3, 3, "(behavior, component) Copies a behaviors values to a component" )
{
    // Fetch behavior/component Ids.
    const S32 behaviorId  = dAtoi( argv[1] );
    const S32 componentId = dAtoi( argv[2] );

    // Fetch behavior.
    BehaviorInstance* pBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( behaviorId ) );

    // Sanity!
    if ( !pBehavior )
    {
        Con::warnf( "copyBehaviorToComponent() - Could not find behavior '%d'.", behaviorId );
        return false;
    }

    // Fetch component.
    SimComponent* pComponent = dynamic_cast<SimComponent*>( Sim::findObject( componentId ) );

    // Sanity!
    if( !pComponent )
    {
        Con::errorf( "copyBehaviorToComponent() -  cannot find component '%d'.", componentId );
        return false;
    }

    // Fetch template.
    BehaviorTemplate* pTemplate = pBehavior->getTemplate();

    // Fetch template field count.
    const U32 fieldCount = pTemplate->getBehaviorFieldCount();
    
    const char* pFieldValue = NULL;
    BehaviorTemplate::BehaviorField* pField = NULL;

    // Copy behavior fields.
    for( U32 index = 0; index < fieldCount; ++index )
    {
        // Fetch behavior field.
        pField = pTemplate->getBehaviorField( index );

        // Fetch field value from behavior (if any).
        pFieldValue = pBehavior->getDataField( pField->mName, NULL );

        // Set field value on component.
        pComponent->setDataField( pField->mName, NULL, pFieldValue );
    }

    return true;
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, addBehavior, bool, 3, 3, "(BehaviorInstance bi) - Add a behavior to the object\n"
                                                           "@param bi The behavior instance to add"
                                                           "@return (bool success) Whether or not the behavior was successfully added")
{
   return object->addBehavior( dynamic_cast<BehaviorInstance *>( Sim::findObject( argv[2] ) ) );
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, removeBehavior, bool, 3, 4, "(BehaviorInstance bi, [bool deleteBehavior = true])\n"
                                                              "@param bi The behavior instance to remove\n"
                                                              "@param deleteBehavior Whether or not to delete the behavior\n"
                                                              "@return (bool success) Whether the behavior was successfully removed")
{
   bool deleteBehavior = true;
   if (argc > 3)
      deleteBehavior = dAtob(argv[3]);

   return object->removeBehavior( dynamic_cast<BehaviorInstance *>( Sim::findObject( argv[2] ) ), deleteBehavior );
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, clearBehaviors, void, 2, 2, "() - Clear all behavior instances\n"
              "@return No return value")
{
   object->clearBehaviors();
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, getBehaviorCount, S32, 2, 2, "() - Get the count of behaviors on an object\n"
                                                               "@return (int count) The number of behaviors on an object")
{
   return object->getBehaviorCount();
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, getBehavior, S32, 3, 3, "(string BehaviorTemplateName) - gets a behavior\n"
                                                          "@param BehaviorTemplateName The name of the template of the behavior instance you want\n"
                                                          "@return (BehaviorInstance bi) The behavior instance you requested")
{
   BehaviorInstance* pBehaviorInstance = object->getBehavior( StringTable->insert( argv[2] ) );

   return pBehaviorInstance ? pBehaviorInstance->getId() : 0;
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, getBehaviorByIndex, S32, 3, 3, "(int index) - Gets a particular behavior\n"
                                                                 "@param index The index of the behavior to get\n"
                                                                 "@return (BehaviorInstance bi) The behavior instance you requested")
{
   BehaviorInstance *bInstance = object->getBehavior( dAtoi(argv[2]) );

   return ( bInstance != NULL ) ? bInstance->getId() : 0;
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, reOrder, bool, 3, 3, "(BehaviorInstance inst, [int desiredIndex = 0])\n"
                                                       "@param inst The behavior instance you want to reorder\n"
                                                       "@param desiredIndex The index you want the behavior instance to be reordered to\n"
                                                       "@return (bool success) Whether or not the behavior instance was successfully reordered" )
{
   BehaviorInstance *inst = dynamic_cast<BehaviorInstance *>( Sim::findObject( argv[1] ) );

   if( inst == NULL )
      return false;

   U32 idx = 0;
   if( argc > 2 )
      idx = dAtoi( argv[2] );

   return object->reOrder( inst, idx );
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, connect, bool, 6, 6,  "(outputBehavior, inputBehavior, outputName, inputName) - Connects a behavior output to a behavior input.\n"
                                                        "@param outputBehavior The behavior that owns the output.\n"
                                                        "@param inputBehavior The behavior that owns the input.\n"
                                                        "@param outputName The output name owned by the output behavior.\n"
                                                        "@param inputName The input name owned by the input behavior.\n"
                                                        "@return (bool success) Whether the connection was successful or not.\n")
{
    // Find output behavior.
    BehaviorInstance* pOutputBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( argv[2] ) );

    // Did we find the behavior?
    if ( !pOutputBehavior )
    {
        // No, so warn.
        Con::warnf("BehaviorComponent::connect() - Could not find output behavior '%s'.", argv[2] );
        return false;
    }

    // Find input behavior.
    BehaviorInstance* pInputBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( argv[3] ) );

    // Did we find the behavior?
    if ( !pInputBehavior )
    {
        // No, so warn.
        Con::warnf("BehaviorComponent::connect() - Could not find input behavior '%s'.", argv[3] );
        return false;
    }

    // Fetch port names.
    StringTableEntry pOutputName = StringTable->insert( argv[4] );
    StringTableEntry pInputName = StringTable->insert( argv[5] );

    // Perform the connection.
    return object->connect( pOutputBehavior, pInputBehavior, pOutputName, pInputName );
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, disconnect, bool, 6, 6,   "(outputBehavior, inputBehavior, outputName, inputName) - Connects a behavior output to a behavior input.\n"
                                                            "@param outputBehavior The behavior that owns the output.\n"
                                                            "@param inputBehavior The behavior that owns the input.\n"
                                                            "@param outputName The output name owned by the output behavior.\n"
                                                            "@param inputName The input name owned by the input behavior.\n"
                                                            "@return (bool success) Whether the disconnection was successful or not.\n")
{
    // Find output behavior.
    BehaviorInstance* pOutputBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( argv[2] ) );

    // Did we find the behavior?
    if ( !pOutputBehavior )
    {
        // No, so warn.
        Con::warnf("BehaviorComponent::disconnect() - Could not find output behavior '%s'.", argv[2] );
        return false;
    }

    // Find input behavior.
    BehaviorInstance* pInputBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( argv[3] ) );

    // Did we find the behavior?
    if ( !pInputBehavior )
    {
        // No, so warn.
        Con::warnf("BehaviorComponent::disconnect() - Could not find input behavior '%s'.", argv[3] );
        return false;
    }

    // Fetch port names.
    StringTableEntry pOutputName = StringTable->insert( argv[4] );
    StringTableEntry pInputName = StringTable->insert( argv[5] );

    // Perform the disconnection.
    return object->disconnect( pOutputBehavior, pInputBehavior, pOutputName, pInputName );
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, raise, bool, 4, 5,    "(outputBehavior, outputName, [deltaTime]) - Raise a signal on the behavior output on the specified behavior.\n"
                                                        "@param outputBehavior The behavior that owns the output.\n"
                                                        "@param outputName The output name owned by the output behavior.\n"
                                                        "@param [deltaTime] Optional time-delta (ms) when the raise should occur.\n"
                                                        "@return (bool success) Whether the signal raise was successful or not.\n")
{
    // Find output behavior.
    BehaviorInstance* pOutputBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( argv[2] ) );

    // Did we find the behavior?
    if ( !pOutputBehavior )
    {
        // No, so warn.
        Con::warnf("BehaviorComponent::raise() - Could not find output behavior '%s'.", argv[2] );
        return false;
    }

    // Fetch output name.
    StringTableEntry pOutputName = StringTable->insert( argv[3] );

    // Perform the signal raising immediately if no schedule time specified.
    if ( argc < 5 )
        return object->raise( pOutputBehavior, pOutputName );

    // Fetch time delta.
    const U32 timeDelta = U32( dAtoi(argv[4]) );

    // Schedule raise event.
    BehaviorComponentRaiseEvent* pEvent = new BehaviorComponentRaiseEvent( pOutputBehavior, pOutputName );
    Sim::postEvent( object, pEvent, Sim::getCurrentTime() + timeDelta );

    return true;
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, getBehaviorConnectionCount, S32, 4, 4,    "(outputBehavior, outputName) - Gets the number of connections on the behavior output on the specified behavior.\n"
                                                                            "@param outputBehavior The behavior that owns the output.\n"
                                                                            "@param outputName The output name owned by the output behavior.\n"
                                                                            "@return The number of connections on the behavior output on the specified behavior.\n")
{
    // Find output behavior.
    BehaviorInstance* pOutputBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( argv[2] ) );

    // Did we find the behavior?
    if ( !pOutputBehavior )
    {
        // No, so warn.
        Con::warnf("BehaviorComponent::getBehaviorConnectionCount() - Could not find output behavior '%s'.", argv[2] );
        return false;
    }

    // Fetch output name.
    StringTableEntry pOutputName = StringTable->insert( argv[3] );

    // Return the connection count.
    return object->getBehaviorConnectionCount( pOutputBehavior, pOutputName );
}

//-----------------------------------------------------------------------------

ConsoleMethod( BehaviorComponent, getBehaviorConnection, const char*, 5, 5,    "(outputBehavior, outputName, connectionIndex) - Gets a comma-delimited list of connections on the behavior output on the specified behavior.\n"
                                                                                "@param outputBehavior The behavior that owns the output.\n"
                                                                                "@param outputName The output name owned by the output behavior.\n"
                                                                                "@param connectionIndex The connection index.\n"
                                                                                "@return Returns a comma-delimited list of connections on the behavior output on the specified behavior of the format <OutputBehavior>,<InputBehavior>,<OutputName>,<InputName>.\n")
{
    // Find output behavior.
    BehaviorInstance* pOutputBehavior = dynamic_cast<BehaviorInstance*>( Sim::findObject( argv[2] ) );

    // Did we find the behavior?
    if ( !pOutputBehavior )
    {
        // No, so warn.
        Con::warnf("BehaviorComponent::getBehaviorConnections() - Could not find output behavior '%s'.", argv[2] );
        return false;
    }

    // Fetch output name.
    StringTableEntry pOutputName = StringTable->insert( argv[3] );

    // Fetch connection index.
    const U32 connectionIndex = dAtoi( argv[4] );

    // Fetch connection.
    const BehaviorComponent::BehaviorPortConnection* pBehaviorConnection = object->getBehaviorConnection( pOutputBehavior, pOutputName, connectionIndex );

    // Finish if there are on connections.
    if ( pBehaviorConnection == NULL )
        return StringTable->EmptyString;

    // Format and return behavior input.
    char* pBuffer = Con::getReturnBuffer(1024);
    dSprintf(pBuffer, 1024, "%d,%d,%s,%s",
        pBehaviorConnection->mOutputInstance->getId(),
        pBehaviorConnection->mInputInstance->getId(),
        pBehaviorConnection->mOutputName,
        pBehaviorConnection->mInputName );
    return pBuffer;
}

