;;; refer-mode.el -- Major mode for refer(1) files
;;
;; Copyright (C) 2008 Jörgen Grahn <grahn+src@snipabacken.se>
;; All right reserved.
;;
;; $Id: allergy-mode.el,v 1.2 2010-01-18 21:27:55 grahn Exp $
;; $Name:  $

(defconst allergy-font-lock-keywords
  '(("^#.*$" . font-lock-comment-face)
    ("\\[.+?\\]" . font-lock-keyword-face)
    ("\{.+?\}" . font-lock-string-face)
    ("^ibid" . font-lock-string-face)
    ("20[0-9][0-9]-..-.. [0-9:]+" . font-lock-variable-name-face)
    ("XXX.*$" . font-lock-warning-face)
    )
  "Syntax highlighting for Allergy mode.")

(define-derived-mode allergy-mode text-mode "Allergy"
  "Major mode for editing allergy photo descriptions.
This is merely text mode, plus some colorization."

  (setq comment-start "#")
  (setq comment-start-skip "#\\s-*")

  (set (make-local-variable 'font-lock-defaults)
       '((allergy-font-lock-keywords
 	  allergy-font-lock-keywords
 	  allergy-font-lock-keywords
 	  allergy-font-lock-keywords)
 	 t)))

(provide 'allergy-mode)
