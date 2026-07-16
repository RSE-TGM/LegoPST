#!/bin/ksh
#
rm -f ${KWIN}/mgw.list
if [ -f ${KWIN}/*.ListVert ]
then
print "@#L@TITLE\t@#L@MGWDESCR\t@#L@HEIGH\t@#L@GTYPE\t@#L@GBACKGROUND\
\t@#L@1KKS\t@#L@1DESCR\t@#L@1PAGE\t@#L@1VAR0\t@#L@1VAR1\t@#L@1VAR2\t@#L@1VAR3\
\t@#L@2KKS\t@#L@2DESCR\t@#L@2PAGE\t@#L@2VAR0\t@#L@2VAR1\t@#L@2VAR2\t@#L@2VAR3\
\t@#L@3KKS\t@#L@3DESCR\t@#L@3PAGE\t@#L@3VAR0\t@#L@3VAR1\t@#L@3VAR2\t@#L@3VAR3\
\t@#L@4KKS\t@#L@4DESCR\t@#L@4PAGE\t@#L@4VAR0\t@#L@4VAR1\t@#L@4VAR2\t@#L@4VAR3\
\t@#L@5KKS\t@#L@5DESCR\t@#L@5PAGE\t@#L@5VAR0\t@#L@5VAR1\t@#L@5VAR2\t@#L@5VAR3\
\t@#L@6KKS\t@#L@6DESCR\t@#L@6PAGE\t@#L@6VAR0\t@#L@6VAR1\t@#L@6VAR2\t@#L@6VAR3\
\t@#L@7KKS\t@#L@7DESCR\t@#L@7PAGE\t@#L@7VAR0\t@#L@7VAR1\t@#L@7VAR2\t@#L@7VAR3\
\t@#L@8KKS\t@#L@8DESCR\t@#L@8PAGE\t@#L@8VAR0\t@#L@8VAR1\t@#L@8VAR2\t@#L@8VAR3\
\t@#L@9KKS\t@#L@9DESCR\t@#L@9PAGE\t@#L@9VAR0\t@#L@9VAR1\t@#L@9VAR2\t@#L@9VAR3\
\t@#L@10KKS\t@#L@10DESCR\t@#L@10PAGE\t@#L@10VAR0\t@#L@10VAR1\t@#L@10VAR2\t@#L@10VAR3\
\t@#L@11KKS\t@#L@11DESCR\t@#L@11PAGE\t@#L@11VAR0\t@#L@11VAR1\t@#L@11VAR2\t@#L@11VAR3\
\t@#L@12KKS\t@#L@12DESCR\t@#L@12PAGE\t@#L@12VAR0\t@#L@12VAR1\t@#L@12VAR2\t@#L@12VAR3\
\t@#L@13KKS\t@#L@13DESCR\t@#L@13PAGE\t@#L@13VAR0\t@#L@13VAR1\t@#L@13VAR2\t@#L@13VAR3\
\t@#L@14KKS\t@#L@14DESCR\t@#L@14PAGE\t@#L@14VAR0\t@#L@14VAR1\t@#L@14VAR2\t@#L@14VAR3\
\t@#L@15KKS\t@#L@15DESCR\t@#L@15PAGE\t@#L@15VAR0\t@#L@15VAR1\t@#L@15VAR2\t@#L@15VAR3\
\t@#L@16KKS\t@#L@16DESCR\t@#L@16PAGE\t@#L@16VAR0\t@#L@16VAR1\t@#L@16VAR2\t@#L@16VAR3\
\t@#L@17KKS\t@#L@17DESCR\t@#L@17PAGE\t@#L@17VAR0\t@#L@17VAR1\t@#L@17VAR2\t@#L@17VAR3" > ${KWIN}/mgw.list
#
ls ${KWIN}/*.ListVert | while read file
do
paste -s ${file} >> ${KWIN}/mgw.list
done
else
echo "Warning : Files ${KWIN}/*.ListVert not found"
fi
