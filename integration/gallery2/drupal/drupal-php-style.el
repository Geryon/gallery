(defconst php-gallery-style
  '(
    (c-basic-offset . 2)
    (my-c-continuation-offset . 2)
    (c-comment-only-line-offset . (0 . 0))
    (c-offsets-alist . ((inline-open . 0)
			(topmost-intro-cont    . 0)
			(statement-block-intro . +)
			(knr-argdecl-intro     . 5)
			(substatement-open     . +)
			(label                 . 0)
			(arglist-intro         . +)
;			(arglist-cont          . c-lineup-arglist-intro-after-paren)
			(arglist-cont          . 0)
			(arglist-close         . c-lineup-arglist)
			(statement-case-open   . +)
			(statement-cont        . +)
			(access-label          . 0)
			(arglist-cont-nonempty . c-lineup-arglist-intro-after-paren)
			))
    (c-echo-syntactic-information-p . t)     ; turn this on to get debug info
    )
  "PHP Style for the Gallery Project")
