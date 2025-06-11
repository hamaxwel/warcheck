#pragma once
#include <vector>
#include <map>
#include <string>

// Example enums for squad formation and role
enum class ESquadFormation {
    Line,
    Column,
    Wedge,
    Vee
};

enum class ESquadRole {
    Leader,
    Point,
    Flank,
    Rear,
    Support
};

struct CommandData {
    std::string Type;
    std::string Target;
    float Priority;
    bool IsUrgent;
};

struct SquadMemberData {
    void* Member; // was AActor*
    ESquadRole Role;
    // Add other fields as needed
};

struct SquadData {
    void* Leader; // was AActor*
    std::vector<SquadMemberData> Members;
    std::vector<CommandData> CommandQueue;
    ESquadFormation Formation;
    float FormationSpacing;
};

class ICTacticalCommandManager {
public:
    ICTacticalCommandManager();
    void BeginPlay();
    void Tick(float deltaTime);
    void CreateSquad(void* leader);
    void AddToSquad(void* member, void* squadLeader);
    void RemoveFromSquad(void* member);
    void SetSquadFormation(void* squadLeader, ESquadFormation newFormation);
    void AssignSquadRole(void* member, ESquadRole newRole);
    void IssueCommand(const CommandData& command, void* target);
    void IssueSquadCommand(const CommandData& command, void* squadLeader);
    bool ValidateCommand(const CommandData& command, void* target) const;
    void ProcessCommandQueue();
    std::vector<void*> GetSquadMembers(void* squadLeader) const;
    bool IsInSquad(void* member) const;
    ESquadFormation GetSquadFormation(void* squadLeader) const;
    ESquadRole GetSquadRole(void* member) const;

protected:
    std::map<void*, SquadData> Squads;
    std::map<void*, std::vector<CommandData>> CommandQueues;
    float DefaultFormationSpacing;
}; 