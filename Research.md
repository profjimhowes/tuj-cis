## A. Measurement foundations (the "scoring is noisy" rock)

**A1 — Classical Test Theory.** Spearman (1904) gives you the true-score-plus-error decomposition your entire Section 1 noise argument rests on; the lineage runs through Kuder–Richardson, Cronbach's α (1951), and Gulliksen's *Theory of Mental Tests* (1950). This bounds "reliability," "standard error of measurement," and the century-old repeated-measurement remedy you invoke.

**A2 — Item Response Theory.** Lord & Novick, *Statistical Theories of Mental Test Scores* (1968) is the founding text; Rasch (1960) and Birnbaum's chapters bound the 1PL/2PL/3PL models, item/test *information*, and parameter invariance. This is the formal home of "more signal per item" and of DAR-as-difficulty.

**A3 — Generalizability Theory.** Cronbach, Gleser, Nanda & Rajaratnam (1972). **This is your most under-exploited lineage.** G-theory decomposes error variance into multiple *facets* (items, occasions, raters, and their interactions) — which is precisely the formal apparatus for your structure×load factor separation in Layer 5 and your "which facet does the variance load on" instinct. Right now your draft reaches for a bespoke measurement model; G-theory is the off-the-shelf ancestor you should either adopt or explicitly improve upon, or a reviewer will ask why you didn't.

**A4 — Cognitive Diagnostic / Diagnostic Classification Models.** Tatsuoka's Rule Space (1983) and the **Q-matrix**, then DINA/DINO/G-DINA (de la Torre) and the Attribute Hierarchy Method (Leighton & Gierl). This is the psychometric formalization of "ship the map, not the grade" — the credential-as-attribute-profile — and it sits directly between KST and IRT. You should know exactly why you're choosing KST's lattice over the Q-matrix formulation, because they're competing dialects for the same object.

**A5 — Knowledge/Competence Space Theory.** Doignon & Falmagne (1985; book 1999); competence-based and procedural KST via Heller, Stefanutti, Albert. You track this deeply already. Bounds ALEKS.

**A6 — Computerized Adaptive Testing.** Lord's later work, Weiss, and Wainer's *Computerized Adaptive Testing: A Primer*. Exposure control (Sympson–Hetter), pool depletion, and the 1994 GRE reconstruction episode live here — bounding your "adaptivity makes item consumption voracious" claim.

## B. Validity & task design (what a certification claim *means*)

**B1 — Validity theory.** Cronbach & Meehl (1955, construct validity) → Messick (1989, the unified/consequential view) → Kane's argument-based validity (2006/2013). **Second high-leverage gap.** Your "isomorphism is tested, not asserted" and your silent-failure-below-the-floor argument are *validity* claims in disguise. Messick's consequential validity and Kane's interpretation/use argument give you the vocabulary to say precisely what breaks when a generator launders memorization — and to defend a credential to an accreditor. Without this scaffolding the silent-failure section reads as intuition rather than theory.

**B2 — Evidence-Centered Design.** Mislevy, Steinberg & Almond (1999–2003). You track this. Student/evidence/task models = your Layers 2–4.

**B3 — Automatic Item Generation.** Bejar (1990s), Embretson's cognitive design systems (1998), Gierl & Haladyna's *AIG* volume (2013). Item models and isomorphs are your Layer 4–5 direct ancestors. **The live front is here**: LLM-based item generation exploded 2023–2026 and a chunk of it postdates my cutoff — this is the single lineage where a search pass is genuinely mandatory rather than optional, because your entire "competency floor / silent failure" moat is an argument about *why most of that recent work is inadequate*, and you need the current state of the art to make that argument land.

**B4 — Standard setting.** Angoff, Bookmark, Nedelsky. Bounds cut scores and "local judgment, rented identity" — how a pass threshold gets defended.

## C. Learning science (why unlimited retakes are learning, not just cheap measurement)

**C1 — Mastery learning & PSI.** Bloom (1968, 1984), Keller (1968), the Kulik meta-analyses. Deeply tracked already.

**C2 — Programmed instruction / behaviorist prehistory.** Skinner's teaching machines, Crowder's branching. The graveyard's earliest headstone.

**C3 — Formative assessment & feedback.** Black & Wiliam (1998), Hattie & Timperley (2007), and — critically — **Kluger & DeNisi (1996), Feedback Intervention Theory.** *This is the empirical ancestor of your loop-viability threshold.* Kluger & DeNisi's headline finding is that roughly a third of feedback interventions *reduce* performance. Your "critical slip rate ε* above which closing the loop is worse than open-loop teaching" is the same phenomenon derived formally instead of meta-analytically. Citing them converts your ε* claim from a toy-model curiosity into a formalization of a known, robust empirical result — a very strong rhetorical position.

**C4 — Retrieval practice, testing effect, spacing.** Roediger & Karpicke (2006), Ebbinghaus, Cepeda et al.'s spacing meta-analysis. Your "one student, 50+ attempts on one item family" observation is the testing effect operating in the wild; this lineage tells you that unlimited un-memorizable retakes aren't just fair, they're the most potent learning intervention known.

**C5 — Expertise & schema acquisition.** de Groot and Chase & Simon (chunking), Ericsson (deliberate practice). Your retrieval/composition dissociation in §4b (the expert sees the operation under the story; the novice sees the story) *is* the expert-novice schema literature. Ground it there explicitly.

**C6 — Transfer.** Thorndike's identical-elements theory through the near/far transfer debate. Directly bounds what "isomorphic variant" can and cannot claim to generalize across — a skeptic's first attack on your radical/incidental distinction.

## D. CS education (the domain-specific rock)

**D1 — Notional machines & mental models.** du Boulay (1986), Sorva (2013). Tracked. Tracing = running the notional machine.

**D2 — The "two populations" debate.** Dehnadi & Bornat's "camel has two humps" (2006), Bornat's own 2014 retraction, and Patitsas et al. (2016) showing grade distributions aren't bimodal. You cite this in your risk section; it's the entire empirical foundation of "talent is partly an artifact of one-shot measurement."

**D3 — Tracing/reading/writing progressions.** The Leeds working group (Lister et al., 2004) on tracing, the BRACElet project, and the trace→explain→write hierarchy, often married to SOLO taxonomy. This is the direct CS-ed corroboration of your Pearl-ladder rung ordering — an independent literature arriving at "you must be able to trace before you can write," which is your rung-2-before-rung-3 claim in different clothing.

**D4 — CS assessment instruments & autograding.** The McCracken multi-institutional study (2001), the FCS1/SCS1 concept inventories (Tew & Guzdial), and the autograder lineage (Web-CAT and successors, plus MOSS for plagiarism). Bounds your behavioral-oracle grading and your reference-case methodology.

**D5 — Debugging as hypothesis-driven cognition.** The program-comprehension and debugging literature (Katz & Anderson and descendants). This is the empirical support for your strongest taxonomic claim — that debugging is rung-3 (abduction + model surgery) and genuinely dissociates from rung-2 tracing.

**D6 — LLMs in CS education.** Finnie-Ansley, Denny, Prather, Becker et al. on Codex/GPT clearing CS1, and the subsequent "AI-proof assessment" scramble. **Live front, postdates cutoff** — a search pass belongs here alongside B3.

## E. Trust economics & credentialing (the third rock)

**E1 — Signaling theory.** Spence (1973), Arrow's filter, and Caplan's *The Case Against Education* (2018) as the maximalist statement. Bounds "the transcript was always the product employers bought."

**E2 — Contract cheating & integrity.** Lancaster & Clarke, Bretag's handbook. Bounds your open-identity-channel argument and the ALEKS cheating-market demonstration.

**E3 — Gatekeeper failure & conflict of interest.** Coffee's gatekeeper theory, the ratings-agency post-mortems. This is the formal literature behind your Moody's analogy and the mission-lock necessity.

## F. Formal backbone (your categorical program)

**F1 — Value of information / Bayesian experimental design.** Lindley (1956), Howard's value of information. Your §4a optimal-policy claims (diagnosis worth only what remediation can spend) are VoI theorems.

**F2 — Comparison of experiments.** **Blackwell (1953).** The seminal bound: "informativeness as the Blackwell garbling preorder" is not your coinage to define, it's Blackwell's theorem, and the categorical-inference people (Fritz's Markov categories, FinStoch, Bayesian inversion) are re-deriving it. Cite the original or a reviewer who knows will notice.

**F3 — Channel theory.** Barwise–Seligman. Tracked.

**F4 — Provably secure steganography / covert channels.** Your own turf — Hopper, Langford & von Ahn — for the anti-remote-proctoring argument. This is a contribution you're positioned to *make*, not just cite.

---

**The four highest-leverage gaps**, if I had to rank where the crawl pays off fastest for the venture as currently drafted: **G-theory (A3)** and **validity theory (B1)** because they give you established formal homes for two claims you're currently building from scratch; **Kluger & DeNisi (C3)** because it turns your ε* threshold into a formalization of a famous empirical result rather than a toy-model artifact; and **Blackwell (F2)** because it's the actual name of a preorder you're currently treating as a definition.

Want me to run the verified-link pass next — I'd start with the two live fronts (B3 and D6), search those properly since they postdate my cutoff, and produce a linked, dated reading list for whichever cluster you want to crawl first? That's where searching earns its keep; the historical spine above I'd stand behind as-is.