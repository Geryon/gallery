#!/bin/sh
# $Id: create_po_template.sh 11924 2005-11-05 01:53:07Z jenst $
#
#note: requires xgettext version 0.12.1 or greater
#
#Note: for version 1.4.2, to support email internationalisation, need to 
#add keyword i18n to xgettext call


##### CORE .pot ############
cat copyright-header.txt > gallery-core.pot

xgettext --files-from=filelist-core -LPHP --keyword=gTranslate:2,3 --no-wrap --msgid-bugs-address="gallery-translations@lists.sourceforge.net" -o - | tail +7 >> gallery-core.pot

##### CONFIG .pot
cat copyright-header.txt > gallery-config.pot

xgettext --files-from=filelist-config -LPHP --keyword=gTranslate:2,3 --no-wrap --msgid-bugs-address="gallery-translations@lists.sourceforge.net" -o - | tail +7 >> gallery-config.pot

##### COMMON .pot
cat copyright-header.txt > gallery-common.pot

xgettext --files-from=filelist-common -LPHP --keyword=gTranslate:2,3 --no-wrap --msgid-bugs-address="gallery-translations@lists.sourceforge.net" -o - | tail +7 >> gallery-common.pot
