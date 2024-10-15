#include "tendermint.h"
#include <utils.h>

using namespace std;
namespace ns3
{

    Tendermint::Tendermint()
    {
        Init();
    }
    Tendermint::~Tendermint() {}

    void Tendermint::Init(void)
    {
        m_round = 0;
        m_isNilPrevoted = false;
        m_isPrevoted = false;
        m_isPrecommited = false;
        m_isNilPrecommited = false;
        m_prevotes.clear();
        m_precommits.clear();
        m_nilprevotes.clear();
        m_nilprecommits.clear();
    }
    uint32_t Tendermint::GetRound(void) { return m_round; }
    void Tendermint::SetRound(uint32_t p_round) { m_round = p_round; }

    void Tendermint::SavePropose(const Propose &p_propose)
    {
        m_propose = p_propose;
    }
    void Tendermint::SavePrevote(const PrevoteCommit &p_prevote)
    {
        m_prevotes[p_prevote.id] = p_prevote;

        // cout << "m_isPrevoted : " << m_isPrevoted << endl;
        // cout << "m_total_voting power : " << m_totalVotingPower << endl;

        if (m_isPrevoted == false && m_prevotes.size() > 1)
        {
            float sum = 0;
            for (auto &it : m_prevotes)
            {
                sum += m_validators[it.first];
            }

            // cout << "voting power : " << sum << " > " << (2 * m_totalVotingPower) / 3 << endl;
            // cout << "prevotes  : size  : " << m_prevotes.size() << endl;
            if (sum > (2 * m_totalVotingPower) / 3)
            {
                m_isPrevoted = true;
            }
        }
    }

    bool Tendermint::isPrevoted(void)
    {
        return m_isPrevoted;
    }

    bool Tendermint::isPrecommited(void)
    {
        return m_isPrecommited;
    }

    void Tendermint::SavePrecommit(const PrevoteCommit &p_precommit)
    {
        m_precommits[p_precommit.id] = p_precommit;
        // cout << "commits  : size  : " << m_precommits.size() << endl;
        // cout << "m_isPre : " << m_isPrecommited << endl;
        if (m_isPrecommited == false && m_precommits.size() > 1) // if it is precommited
        {
            float sum = 0;
            for (auto &it : m_precommits)
            {
                sum += m_validators[it.first];
            }
            // cout << "sum  : " << sum << " voting power : " << (2 * m_totalVotingPower) / 3 << endl;
            if ((sum > (2 * m_totalVotingPower) / 3))
            {
                m_isPrecommited = true;
            }
        }
    }

    void Tendermint::SaveNilPrevote(const PrevoteCommitNil &p_nilprevote)
    {
        m_nilprevotes[p_nilprevote.id] = p_nilprevote;

        if (!m_isNilPrevoted) // check if it is prevoted
        {

            float sum = 0;
            for (auto it = m_nilprevotes.begin(); it != m_nilprevotes.end(); it++)
            {
                sum += m_validators[it->first];
            }
            if (sum > (2 / 3) * m_totalVotingPower)
            {
                m_isNilPrevoted = true;
            }
        }
    }

    void Tendermint::SaveNilPrecommit(const PrevoteCommitNil &p_nilprecommit)
    {
        m_nilprecommits[p_nilprecommit.id] = p_nilprecommit;
        if (!m_isNilPrecommited) // if it is precommited
        {
            float sum = 0;
            for (auto it = m_nilprecommits.begin(); it != m_nilprecommits.end(); it++)
            {
                sum += m_validators[it->first];
            }
            if ((sum > (2 / 3) * m_totalVotingPower))
            {
                m_isNilPrecommited = true;
            }
        }
    }

    bool Tendermint::isNilPrecommited(void)
    {
        return m_isNilPrecommited;
    }

    bool Tendermint::isNilPrevoted(void)
    {
        return m_isNilPrevoted;
    }

    void Tendermint::NewRound(void)
    {
        m_round++;
    }

    void Tendermint::SetValidators(const std::map<uint32_t, float> &p_validators)
    {
        m_validators = p_validators;
        m_totalVotingPower = 0;
        for (auto &a : m_validators)
        {
            m_totalVotingPower += a.second;
        }
    }

    map<uint32_t, float> *Tendermint::GetValidators(void)
    {
        return &m_validators;
    }

    Propose Tendermint::GetPropose(void) const
    {
        return m_propose;
    }

    Block *Tendermint::GetProposeBlock(void)
    {
        return &m_propose.block;
    }

    double Tendermint::GetStartTime(void) const
    {
        return m_startTime;
    }

    void Tendermint::SetStartTime(const double &p_start)
    {
        m_startTime = p_start;
    }

    

}