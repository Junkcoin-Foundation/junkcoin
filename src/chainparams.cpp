// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <hash.h> // for signet block challenge hash
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <versionbitsinfo.h>
#include <script/standard.h>
#include <key_io.h>

#include <assert.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Wed May 1, 2013: Spot gold fell 1.3 percent to $1,457.90 an ounce by 3:11 p.m. EDT (1911 GMT)";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 100000; // Not used in Junkcoin (custom schedule)
        consensus.BIP16Height = 0; // Always enforce P2SH
        consensus.BIP34Height = 0x210c; // 8460
        consensus.BIP34Hash = uint256S("0xa2effa738145e377e08a61d76179c21703e13e48910b30a2a87f0dfe794b64c6"); // genesis
        consensus.BIP65Height = 0x210c; // 8460
        consensus.BIP66Height = 0x210c; // 8460
        consensus.CSVHeight = 700000; // Force activation at block 700000 (BIP9 voting won't work due to merge mining)
        // Junkcoin: SegWit scheduled for activation at block 700000
        consensus.SegwitHeight = 700000; // Activate at block 700000
        consensus.MinBIP9WarningHeight = 10080 + 10080; // miner confirmation window
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // 1 day
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Junkcoin: Taproot scheduled for future (not in original Junkcoin)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartHeight = 800000; // Future activation
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeoutHeight = 1000000; // Far future timeout

        // Junkcoin: MWEB scheduled for far future (Litecoin-specific feature)
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nStartHeight = 900000; // Far future
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nTimeoutHeight = 1100000; // Far future timeout

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        consensus.defaultAssumeValid = uint256S("0xa2effa738145e377e08a61d76179c21703e13e48910b30a2a87f0dfe794b64c6"); // genesis

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x2020; // 8224 in decimal
        consensus.fStrictChainId = true;
        consensus.nAuxpowStartHeight = 173000;
        
        // Digishield parameters (NOT ACTIVE - set to never)
        consensus.nHeightEffective = 0xFFFFFFFF; // Digishield not activated

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 9771;
        nPruneAfterHeight = 518400;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(1367394064, 112158625, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xa2effa738145e377e08a61d76179c21703e13e48910b30a2a87f0dfe794b64c6"));
        assert(genesis.hashMerkleRoot == uint256S("0x3de124b0274307911fe12550e96bf76cb92c12835db6cb19f82658b8aca1dbc8"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as an addrfetch if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        // 
        // NOTE: Junkcoin DNS seeds use old format (no service bit filtering)
        // Litecoin's DNS mechanism will try x1.hostname first, then fallback to AddAddrFetch(hostname)
        // This provides backward compatibility with old DNS seed servers
        vSeeds.emplace_back("mainnet.junk-coin.com");
        vSeeds.emplace_back("junk-seed.s3na.xyz");
        vSeeds.emplace_back("jkc-seed.junkiewally.xyz");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,16);  // Legacy addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);   // Script addresses start with '3'
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,5);  // Script addresses
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,144); // WIF private keys start with 'N'
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "jc";
        mweb_hrp = "jcmweb";

        // Fixed seeds disabled - array is empty
        vFixedSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {      0, uint256S("0xa2effa738145e377e08a61d76179c21703e13e48910b30a2a87f0dfe794b64c6")},
                {      1, uint256S("0xca55073a54775a1ef78294f53f38a3e02d0654d7417f3cbbe4d28d17d50e07d0")},
                {     53, uint256S("0xb623a39a5a0534990a59916d5803fa2bd6a6d52d8e594546936a42a2cc9b0441")},
                {    117, uint256S("0x6cab49bd69fcce2bb48793cc064bb49e75f068e7029b5173db83654fbcb5953d")},
                {    200, uint256S("0x45257b0f2ee6d5c55ac16a76817d7151b776d6452ae6f21426eaa42345b831f8")},
                {   6452, uint256S("0x506562c2172d9f10e86d2b467ed3bb7b9eba40148d18d1e660c1ff692604f3fc")},
                {  10978, uint256S("0x1c9f7f7a4702f8225df430b259ac58c387de99439be8a8789841a1c011ead7fc")},
                {  17954, uint256S("0x6036051659e92a17cb7488040e05a94483b7a7f88b184156c136d51ff0390a7d")},
                {  23978, uint256S("0x7924154aa896363ec9be3ca5f939602f72cf4a5396e6e1cd9139335dd1819487")},
                {  33212, uint256S("0x448040ac454da8654d9c58ad79386aa1a88fd113be0fcc5ca39ecd3eae8c8618")},
                {  45527, uint256S("0xf2420d964001d4d2c8bc0d9283f3f684d4d91a509a50985888458a68e08e1c82")},
                {  57484, uint256S("0xc3e95c6fb35f4b39006c89538415b4f50a253a3ac1cad0e583fb287f6bd91be1")},
                {  69240, uint256S("0xc34f5d113fe92f3206ef8855caf51cd6252286e3381b253bbc1237211198c22b")},
                {  73892, uint256S("0xd05129c2d9f3e99565bf84fbceabbc61728e4d644173e194823b639f7c406b04")},
                { 168312, uint256S("0xdeea2bcecb1146ae9cd74d67b29b4d0161e9bb63beb9022ca10f3625dda6c0e6")},
                { 172000, uint256S("0x6208026838cd18d3f401f3c5c2305105a3aafeb63164827d3e341f356cb3529a")},
            }
        };

        chainTxData = ChainTxData{
            /* nTime    */ 0,
            /* nTxCount */ 0,
            /* dTxRate  */ 0
        };

        // Development Fund - 20% of block reward goes to dev fund
        vDevelopmentFundAddress = {
            "3P3UvT6vdDJVrbB2mn6WrP8gywpu2Knx8C",
            "34cGTrxRD4VvfbDri6RhQDKPBokfLTNJse",
            "37NpTG2p6gjVeZDmAiPLKNs6Nhj5EfTR55"
        };
        vDevelopmentFundStartHeight = 365000;
        vDevelopmentFundLastHeight = 3547800;
        vDevelopmentFundPercent = 0.2; // 20% development fund
        assert(static_cast<int>(vDevelopmentFundAddress.size()) <= GetLastDevelopmentFundBlockHeight());
        
        // Junkcoin: Setup consensus tree for height-based consensus rules
        // This allows different consensus parameters at different heights
        
        // Digishield consensus (not activated - set to never)
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 0xFFFFFFFF; // Never activated
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // 1 minute
        
        // Min difficulty consensus (not activated)
        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = false;
        
        // AuxPoW consensus (not activated as separate fork)
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = std::numeric_limits<uint32_t>::max();
        
        // Assemble the binary search tree of consensus parameters
        // Root is digishield, left is base consensus, right is auxpow
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 100000; // Junkcoin (not Litecoin's 840000)
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on testnet
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256S("8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573");
        consensus.BIP65Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.CSVHeight = 6048; // 00000000025e930139bac5c6c31a403776da130831ab85be56578f3fa75369bb
        consensus.SegwitHeight = 6048; // 00000000002b980fcd729daaa248fd9316a5200e9b367f4ff2c42453e84201ca
        consensus.MinBIP9WarningHeight = 8064; // segwit activation height + miner confirmation window
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 24 * 60 * 60; // Junkcoin: 1 day (not Litecoin's 3.5 days)
        consensus.nPowTargetSpacing = 60; // Junkcoin: 1 minute (not Litecoin's 2.5 minutes)
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1080; // 75% of 1440 (Junkcoin testnet)
        consensus.nMinerConfirmationWindow = 1440; // 24 * 60 = 1440 blocks (1 day at 1 minute spacing)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartHeight = 2225664; // March 2022
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeoutHeight = 2435328; // 364 days later

        // Deployment of MWEB (LIP-0002, LIP-0003, and LIP-0004)
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nStartHeight = 2209536; // Jan/Feb 2022
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nTimeoutHeight = 2419200; // 364 days later

        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000004260a1758f04aa");
        consensus.defaultAssumeValid = uint256S("0x4a280c0e150e3b74ebe19618e6394548c8a39d5549fd9941b9c431c73822fbd5"); // 1737876

        // Junkcoin testnet magic bytes and port
        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        nDefaultPort = 19771;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 4;
        m_assumed_chain_state_size = 1;

        // Junkcoin: Testnet uses same genesis as mainnet
        genesis = CreateGenesisBlock(1367394064, 112158625, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xa2effa738145e377e08a61d76179c21703e13e48910b30a2a87f0dfe794b64c6"));
        assert(genesis.hashMerkleRoot == uint256S("0x3de124b0274307911fe12550e96bf76cb92c12835db6cb19f82658b8aca1dbc8"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet-seed.junkcointools.com");
        vSeeds.emplace_back("seed-b.junkcoin.loshan.co.uk");
        vSeeds.emplace_back("dnsseed-testnet.thrasher.io");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tltc";
        mweb_hrp = "tmweb";

        // Empty testnet seeds - use DNS seeds or -addnode
        vFixedSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {300, uint256S("54e6075affe658d6574e04c9245a7920ad94dc5af8f5b37fd9a094e317769740")},
                {2056, uint256S("17748a31ba97afdc9a4f86837a39d287e3e7c7290a08a1d816c5969c78a83289")},
                {2352616, uint256S("7540437e7bf7831fa872ba8cfae85951a1e5dbb04c201b6f5def934d9299f3c2")}
            }
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 4096 36d8ad003bac090cf7bf4e24fbe1d319554c8933b9314188d6096ac12648764d
            /* nTime    */ 1607986972,
            /* nTxCount */ 4229067,
            /* dTxRate  */ 0.06527021772939347,
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in functional tests)
        consensus.CSVHeight = 432; // CSV activated on regtest (Used in rpc activation tests)
        consensus.SegwitHeight = 0; // SEGWIT is always activated on regtest unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 3.5 * 24 * 60 * 60; // 3.5 days
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // Deployment of MWEB (LIP-0002 and LIP-0003)
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].bit = 4;
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nStartTime = 1601450001; // September 30, 2020
        consensus.vDeployments[Consensus::DEPLOYMENT_MWEB].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 19444;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateActivationParametersFromArgs(args);

        // Junkcoin regtest genesis
        genesis = CreateGenesisBlock(1369199888, 12097647, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        // Junkcoin: No assertion for regtest (allows flexible testing)

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData = {
            {
                {0, uint256S("530827f38f93b43ed12af0b3ad25a288dc02ed74d6d7857862df51fc56c416f9")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rltc";
        mweb_hrp = "tmweb";
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout, int64_t nStartHeight, int64_t nTimeoutHeight)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
        consensus.vDeployments[d].nStartHeight = nStartHeight;
        consensus.vDeployments[d].nTimeoutHeight = nTimeoutHeight;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (args.IsArgSet("-segwitheight")) {
        int64_t height = args.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() < 3 || 5 < vDeploymentParams.size()) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end[:heightstart:heightend]");
        }
        int64_t nStartTime, nTimeout, nStartHeight, nTimeoutHeight;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        if (vDeploymentParams.size() > 3 && !ParseInt64(vDeploymentParams[3], &nStartHeight)) {
            throw std::runtime_error(strprintf("Invalid nStartHeight (%s)", vDeploymentParams[3]));
        }
        if (vDeploymentParams.size() > 4 && !ParseInt64(vDeploymentParams[4], &nTimeoutHeight)) {
            throw std::runtime_error(strprintf("Invalid nTimeoutHeight (%s)", vDeploymentParams[4]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout, nStartHeight, nTimeoutHeight);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld, start_height=%d, timeout_height=%d\n", vDeploymentParams[0], nStartTime, nTimeout, nStartHeight, nTimeoutHeight);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN) {
        return std::unique_ptr<CChainParams>(new CMainParams());
    } else if (chain == CBaseChainParams::TESTNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    } else if (chain == CBaseChainParams::SIGNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams()); // TODO: Support SigNet
    } else if (chain == CBaseChainParams::REGTEST) {
        return std::unique_ptr<CChainParams>(new CRegTestParams(args));
    }
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(gArgs, network);
}

// Development Fund helper methods implementation
// Block height must be >= vDevelopmentFundStartHeight and <= vDevelopmentFundLastHeight
// Index variable i ranges from 0 - (vDevelopmentFundAddress.size()-1)
std::string CChainParams::GetDevelopmentFundAddressAtHeight(int nHeight) const {
    assert(nHeight >= vDevelopmentFundStartHeight && nHeight <= vDevelopmentFundLastHeight);
    size_t addressChangeInterval = (vDevelopmentFundLastHeight - vDevelopmentFundStartHeight + 1) / vDevelopmentFundAddress.size();
    size_t i = (nHeight - vDevelopmentFundStartHeight) / addressChangeInterval;
    return vDevelopmentFundAddress[i];
}

// Block height must be >0 and <=last development fund block height
// The development fund address is expected to be a multisig (P2SH) address
CScript CChainParams::GetDevelopmentFundScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= GetLastDevelopmentFundBlockHeight());

    CTxDestination dest = DecodeDestination(GetDevelopmentFundAddressAtHeight(nHeight));
    assert(IsValidDestination(dest));
    
    // v0.21: CScriptID renamed to ScriptHash
    const ScriptHash* scriptHash = boost::get<ScriptHash>(&dest);
    assert(scriptHash != nullptr);
    
    CScript script = CScript() << OP_HASH160 << ToByteVector(*scriptHash) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetDevelopmentFundAddressAtIndex(int i) const {
    assert(i >= 0 && i < static_cast<int>(vDevelopmentFundAddress.size()));
    return vDevelopmentFundAddress[i];
}

/**
 * Junkcoin: Get consensus parameters for a given height using binary search tree.
 * This allows different consensus rules at different heights (e.g., digishield, auxpow).
 */
const Consensus::Params *Consensus::Params::GetConsensus(uint32_t nTargetHeight) const {
    if (nTargetHeight < this->nHeightEffective && this->pLeft != nullptr) {
        return this->pLeft->GetConsensus(nTargetHeight);
    } else if (nTargetHeight > this->nHeightEffective && this->pRight != nullptr) {
        const Consensus::Params *pCandidate = this->pRight->GetConsensus(nTargetHeight);
        if (pCandidate->nHeightEffective <= nTargetHeight) {
            return pCandidate;
        }
    }

    // No better match below the target height
    return this;
}
