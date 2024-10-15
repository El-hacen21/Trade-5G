#ifndef TENDERMINT_H
#define TENDERMINT_H

#include "block.h"
#include <map>
#include <consensus_messages.h>
#include <memory>

namespace ns3
{

    class Tendermint
    {
    public:
        Tendermint();
        ~Tendermint();

        void SavePropose(const Propose &p_propose);
        void SavePrevote(const PrevoteCommit &p_prevote);
        void SavePrecommit(const PrevoteCommit &p_precommit);
        bool isPrecommited(void); // returns true if 2/3 prevotes
        bool isPrevoted(void);    // returns true if 2/3 precommits

        void SaveNilPrevote(const PrevoteCommitNil &p_nilprevote);
        void SaveNilPrecommit(const PrevoteCommitNil &p_nilprecommit);
        bool isNilPrecommited(void); // returns true if 2/3 nilprevotes
        bool isNilPrevoted(void);    // returns true if 2/3 nilprecommits
        void NewRound(void);

        Block* GetProposeBlock(void);

        /*
         * round =0
         * clean pre-prepare, prepares, commit...
         */
        void Init(void);
        uint32_t GetRound(void);
        void SetRound(uint32_t p_round);
        void SetValidators(const std::map<uint32_t, float> &p_validators);
        map<uint32_t, float> *GetValidators(void);

        Propose GetPropose(void) const;
        double GetStartTime(void) const;
        void SetStartTime(const double &p_start);

    private:
        uint32_t m_round;
        double m_startTime;
        Propose m_propose;
        map<uint32_t, PrevoteCommit> m_prevotes;         // counts prevotes
        map<uint32_t, PrevoteCommit> m_precommits;       // counts commits
        map<uint32_t, PrevoteCommitNil> m_nilprevotes;   // counts nil prevotes
        map<uint32_t, PrevoteCommitNil> m_nilprecommits; // counts nil precommits
        map<uint32_t, float> m_validators;               // tendermint validators
        float m_totalVotingPower = 0;                    // total voting power
        bool m_isPrevoted = false;
        bool m_isPrecommited = false;
        bool m_isNilPrevoted = false;
        bool m_isNilPrecommited = false;
    };
}

#endif
