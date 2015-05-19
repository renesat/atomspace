;
; Configuration file for the example crisp rule base system (used by
; simple-deduction.scm)
;
; Before running any inference you must load that file

; Load the rules
(load-from-path "rules/crisp-modus-ponens.scm")
(load-from-path "rules/crisp-deduction.scm")

; Define a new rule base (aka rule-based system)
(InheritanceLink
   (ConceptNode "crisp-rule-base")
   (ConceptNode "URE")
)

; Associate the rules to the rule base (with weights, their semantics
; is currently undefined, we might settled with probabilities but it's
; not sure)
(MemberLink (stv 0.4 1)
   (ConceptNode "crisp-rule-base-modus-ponens-rule")
   (ConceptNode "crisp-rule-base")
)
(MemberLink (stv 0.6 1)
   (ConceptNode "crisp-rule-base-deduction-rule")
   (ConceptNode "crisp-rule-base")
)

; Termination criteria parameters
(ExecutionLink
   (SchemaNode "URE:maximum-iterations")
   (ConceptNode "crisp-rule-base")
   (NumberNode "20")
)

; Other parameters (enable attention allocation)
(ExecutionLink
   (SchemaNode "URE:attention-allocation")
   (ConceptNode "crisp-rule-base")
   (ConceptNode "False") ; "True" or "False", maybe we should create
                         ; an Atomese Boolean Type
)
