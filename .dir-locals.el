((nil .
      ((eval . (set (make-local-variable 'project-path)
                      (file-name-directory
                       (let ((d (dir-locals-find-file "./")))
                         (if (stringp d) d (car d))))))
         (eval . (message "Project directory set to `%s'." project-path))

         (eval . (setq cmake-ide-project-dir project-path))
         (eval . (setq cmake-ide-build-dir (concat project-path "bin")))
         (cmake-ide-cmake-opts . "-GNinja -DCMAKE_BUILD_TYPE=Debug")
         )))
