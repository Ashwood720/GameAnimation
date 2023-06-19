#include "OCLTTraceUtils.h"
#include "DrawDebugHelpers.h"

bool OCLTTraceUtils::SweepCapsuleSingleByChannel(const UWorld* World, FHitResult& OutHit, const FVector& Start, const FVector& End, float CapsuleRadius, float CapsuleHalfHeight, const FQuat& Rot, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, const FCollisionResponseParams& ResponseParam, bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor)
{
    bool bResult = false;
    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    bResult = World->SweepSingleByChannel(OutHit, Start, End, Rot, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
    if (bDrawDebug)
    {
        DrawDebugCapsule(World, Start, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
        DrawDebugCapsule(World, End, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, TraceColor, false, DrawTime);
        DrawDebugLine(World, Start, End, TraceColor, false, DrawTime);

        if (bResult) 
        {
            DrawDebugCapsule(World, OutHit.Location, CapsuleHalfHeight, CapsuleRadius, FQuat::Identity, FColor::Red, false, DrawTime);
            DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, FColor::Red, false, DrawTime);
        }
    }
#endif
    return bResult;
}

bool OCLTTraceUtils::SweepSphereSingleByChannel(const UWorld* World, FHitResult& OutHit, const FVector& Start, const FVector& End, float Radius, ECollisionChannel TraceChannel, const FCollisionQueryParams& Params, const FCollisionResponseParams& ResponseParam, bool bDrawDebug, float DrawTime, FColor TraceColor, FColor HitColor)
{
    bool bResult = false;
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(Radius);
    bResult = World->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, TraceChannel, CollisionShape, Params, ResponseParam);

#if ENABLE_DRAW_DEBUG
    if (bDrawDebug)
    {
        FVector DebugCenter = (Start + End) * 0.5f;
        FVector TraceVector = End - Start;
        float DebugCapsuleHalfHeight = TraceVector.Size() * 0.5f;
        
        FQuat DebugCapsuleRotation = FRotationMatrix::MakeFromZ(TraceVector).ToQuat();

        DrawDebugCapsule(World, DebugCenter, DebugCapsuleHalfHeight, Radius, DebugCapsuleRotation, TraceColor, false, DrawTime);

        if (bDrawDebug)
        {
            DrawDebugSphere(World, OutHit.Location, Radius, 16, HitColor, false, DrawTime);
            DrawDebugPoint(World, OutHit.ImpactPoint, 10.0f, HitColor, false, DrawTime);
        }
    }
#endif
    return bResult;
}

bool OCLTTraceUtils::OverlapCapsuleAnyByProfile(const UWorld* World, const FVector& Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, bool bDrawDebug, float DrawTime, FColor HitColor)
{
    bool bResults = false;

    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    bResults = World->OverlapAnyTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);
#if ENABLE_DRAW_DEBUG   
    if (bDrawDebug && bResults) 
    {
        DrawDebugCapsule(World, Pos, CapsuleHalfHeight, CapsuleRadius, Rotation, HitColor , false, DrawTime);
    }
#endif
    return bResults;
}

bool OCLTTraceUtils::OverlapCapsuleBlockingByProfile(const UWorld* World, const FVector& Pos, float CapsuleRadius, float CapsuleHalfHeight, FQuat Rotation, FName ProfileName, const FCollisionQueryParams& QueryParams, bool bDrawDebug, float DrawTime, FColor HitColor)
{
    bool bResults = false;

    FCollisionShape CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
    bResults = World->OverlapBlockingTestByProfile(Pos, Rotation, ProfileName, CollisionShape, QueryParams);
#if ENABLE_DRAW_DEBUG   
    if (bDrawDebug && bResults)
    {
        DrawDebugCapsule(World, Pos, CapsuleHalfHeight, CapsuleRadius, Rotation, HitColor, false, DrawTime);
    }
#endif
    return bResults;
}

bool OCLTTraceUtils::OverlapLineTraceSingleByProfile(const UWorld* World, const FVector& LineStart, const FVector& LineEnd, FName ProfileName, const struct FCollisionQueryParams& Params, bool bDrawDebug, float DrawTime, FColor HitColor)
{
    bool bResults = false;

    bResults = World->LineTraceTestByProfile(LineStart, LineEnd, ProfileName, Params);
#if ENABLE_DRAW_DEBUG   
    if (bDrawDebug && bResults)
    {
        DrawDebugLine(World, LineStart, LineEnd, HitColor, false, 0, 0.1f);
    }
#endif
    return bResults;
}
