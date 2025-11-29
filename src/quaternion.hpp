#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>
#include <iostream>
#include <stdexcept>

/**
 * @class Quaternion
 * @brief Quaternion implementation for 3D rotations
 * 
 * Un quaternion est représenté sous la forme q = w + xi + yj + zk
 * où w est la partie scalaire et (x, y, z) est la partie vectorielle
 */
class Quaternion {
private:
    double w, x, y, z;

public:
    // ==================== CONSTRUCTEURS ====================
    
    /**
     * @brief Constructeur par défaut - quaternion identité (pas de rotation)
     */
    Quaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {}
    
    /**
     * @brief Constructeur avec composantes
     */
    Quaternion(double w, double x, double y, double z) 
        : w(w), x(x), y(y), z(z) {}
    
    /**
     * @brief Crée un quaternion à partir d'un axe et d'un angle (représentation physique)
     * @param axis_x, axis_y, axis_z: composantes de l'axe de rotation (normalisé automatiquement)
     * @param angle: angle de rotation en radians
     */
    static Quaternion fromAxisAngle(double axis_x, double axis_y, double axis_z, double angle) {
        double half_angle = angle * 0.5;
        double sin_half = std::sin(half_angle);
        
        // Normalisation de l'axe
        double norm = std::sqrt(axis_x*axis_x + axis_y*axis_y + axis_z*axis_z);
        if (norm < 1e-10) {
            // Axe nul -> quaternion identité
            return Quaternion(1.0, 0.0, 0.0, 0.0);
        } else {
            double w = std::cos(half_angle);
            double x = (axis_x / norm) * sin_half;
            double y = (axis_y / norm) * sin_half;
            double z = (axis_z / norm) * sin_half;
            return Quaternion(w, x, y, z);
        }
    }
    
    // ==================== ACCESSEURS ====================
    
    double getW() const { return w; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    
    void set(double w_, double x_, double y_, double z_) {
        w = w_; x = x_; y = y_; z = z_;
    }
    
    // ==================== OPÉRATIONS FONDAMENTALES ====================
    
    /**
     * @brief Calcule la norme (magnitude) du quaternion
     */
    double norm() const {
        return std::sqrt(w*w + x*x + y*y + z*z);
    }
    
    /**
     * @brief Normalise le quaternion (le rend unitaire)
     * Essentiel pour représenter des rotations pures
     */
    void normalize() {
        double n = norm();
        if (n < 1e-10) {
            throw std::runtime_error("Cannot normalize zero quaternion");
        }
        w /= n; x /= n; y /= n; z /= n;
    }
    
    /**
     * @brief Retourne une copie normalisée
     */
    Quaternion normalized() const {
        Quaternion result = *this;
        result.normalize();
        return result;
    }
    
    /**
     * @brief Calcule le conjugué: q* = w - xi - yj - zk
     * Important: pour un quaternion unitaire, q* = q⁻¹
     */
    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }
    
    /**
     * @brief Calcule l'inverse: q⁻¹ = q* / |q|²
     */
    Quaternion inverse() const {
        double n2 = w*w + x*x + y*y + z*z;
        if (n2 < 1e-10) {
            throw std::runtime_error("Cannot invert zero quaternion");
        }
        return Quaternion(w/n2, -x/n2, -y/n2, -z/n2);
    }
    
    // ==================== OPÉRATEURS ====================
    
    /**
     * @brief Multiplication de quaternions (composition de rotations)
     * ATTENTION: l'ordre est important! q1 * q2 ≠ q2 * q1
     * q1 * q2 signifie: appliquer d'abord q2, puis q1
     */
    Quaternion operator*(const Quaternion& q) const {
        return Quaternion(
            w*q.w - x*q.x - y*q.y - z*q.z,
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y - x*q.z + y*q.w + z*q.x,
            w*q.z + x*q.y - y*q.x + z*q.w
        );
    }
    
    /**
     * @brief Addition de quaternions
     */
    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
    }
    
    /**
     * @brief Soustraction de quaternions
     */
    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
    }
    
    /**
     * @brief Multiplication par un scalaire
     */
    Quaternion operator*(double scalar) const {
        return Quaternion(w*scalar, x*scalar, y*scalar, z*scalar);
    }
    
    /**
     * @brief Division par un scalaire
     */
    Quaternion operator/(double scalar) const {
        if (std::abs(scalar) < 1e-10) {
            throw std::runtime_error("Division by zero");
        }
        return Quaternion(w/scalar, x/scalar, y/scalar, z/scalar);
    }
    
    // ==================== ROTATION DE VECTEURS ====================
    
    /**
     * @brief Applique la rotation représentée par ce quaternion à un vecteur 3D
     * Formule: v' = q * v * q*
     * où v est traité comme un quaternion pur (partie scalaire = 0)
     * 
     * @param vx, vy, vz: composantes du vecteur à tourner
     * @param rx, ry, rz: références pour stocker le résultat
     */
    void rotateVector(double vx, double vy, double vz,
                     double& rx, double& ry, double& rz) const {
        // Méthode optimisée sans créer de quaternions intermédiaires
        double ww = w * w;
        double xx = x * x;
        double yy = y * y;
        double zz = z * z;
        double wx = w * x;
        double wy = w * y;
        double wz = w * z;
        double xy = x * y;
        double xz = x * z;
        double yz = y * z;
        
        rx = vx * (ww + xx - yy - zz) + 2.0 * (vy * (xy - wz) + vz * (xz + wy));
        ry = vy * (ww - xx + yy - zz) + 2.0 * (vx * (xy + wz) + vz * (yz - wx));
        rz = vz * (ww - xx - yy + zz) + 2.0 * (vx * (xz - wy) + vy * (yz + wx));
    }
    
    // ==================== CONVERSIONS ====================
    
    /**
     * @brief Extrait l'axe et l'angle de rotation
     * @param axis_x, axis_y, axis_z: références pour l'axe normalisé
     * @param angle: référence pour l'angle en radians
     */
    void toAxisAngle(double& axis_x, double& axis_y, double& axis_z, double& angle) const {
        double w_clamped = (w < -1.0) ? -1.0 : (w > 1.0 ? 1.0 : w);
        angle = 2.0 * std::acos(w_clamped);
        double sin_half = std::sin(angle * 0.5);
        
        if (std::abs(sin_half) < 1e-10) {
            // Pas de rotation ou rotation de 360°
            axis_x = 1.0; axis_y = 0.0; axis_z = 0.0;
        } else {
            axis_x = x / sin_half;
            axis_y = y / sin_half;
            axis_z = z / sin_half;
        }
    }
    
    /**
     * @brief Convertit en matrice de rotation 3x3
     * @param mat: tableau de 9 éléments (row-major order)
     */
    void toRotationMatrix(double mat[9]) const {
        double ww = w * w;
        double xx = x * x;
        double yy = y * y;
        double zz = z * z;
        double wx = w * x;
        double wy = w * y;
        double wz = w * z;
        double xy = x * y;
        double xz = x * z;
        double yz = y * z;
        
        mat[0] = ww + xx - yy - zz;
        mat[1] = 2.0 * (xy - wz);
        mat[2] = 2.0 * (xz + wy);
        
        mat[3] = 2.0 * (xy + wz);
        mat[4] = ww - xx + yy - zz;
        mat[5] = 2.0 * (yz - wx);
        
        mat[6] = 2.0 * (xz - wy);
        mat[7] = 2.0 * (yz + wx);
        mat[8] = ww - xx - yy + zz;
    }
    
    /**
     * @brief Convertit en angles d'Euler (roll, pitch, yaw) en radians
     * Convention: rotation ZYX (yaw, puis pitch, puis roll)
     */
    void toEulerAngles(double& roll, double& pitch, double& yaw) const {
        // Roll (rotation autour de X)
        double sinr_cosp = 2.0 * (w * x + y * z);
        double cosr_cosp = 1.0 - 2.0 * (x * x + y * y);
        roll = std::atan2(sinr_cosp, cosr_cosp);
        
        // Pitch (rotation autour de Y)
        double sinp = 2.0 * (w * y - z * x);
        if (std::abs(sinp) >= 1.0)
            pitch = std::copysign(M_PI / 2.0, sinp); // Gimbal lock
        else
            pitch = std::asin(sinp);
        
        // Yaw (rotation autour de Z)
        double siny_cosp = 2.0 * (w * z + x * y);
        double cosy_cosp = 1.0 - 2.0 * (y * y + z * z);
        yaw = std::atan2(siny_cosp, cosy_cosp);
    }
    
    // ==================== INTERPOLATION ====================
    
    /**
     * @brief Interpolation linéaire sphérique (SLERP)
     * Interpolation la plus naturelle entre deux rotations
     * 
     * @param q1: quaternion de départ
     * @param q2: quaternion d'arrivée
     * @param t: paramètre d'interpolation [0, 1]
     * @return quaternion interpolé
     */
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, double t) {
        // Produit scalaire pour calculer l'angle entre les quaternions
        double dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
        
        // Si le produit scalaire est négatif, prendre le chemin le plus court
        Quaternion q2_adjusted = q2;
        if (dot < 0.0) {
            q2_adjusted = q2 * (-1.0);
            dot = -dot;
        }
        
        // Si les quaternions sont très proches, utiliser l'interpolation linéaire
        if (dot > 0.9995) {
            Quaternion result = q1 + (q2_adjusted - q1) * t;
            result.normalize();
            return result;
        }
        
        // Calcul de SLERP
        double dot_clamped = (dot < -1.0) ? -1.0 : (dot > 1.0 ? 1.0 : dot);
        double theta = std::acos(dot_clamped);
        double sin_theta = std::sin(theta);
        double w1 = std::sin((1.0 - t) * theta) / sin_theta;
        double w2 = std::sin(t * theta) / sin_theta;
        
        return q1 * w1 + q2_adjusted * w2;
    }
    
    // ==================== INTÉGRATION TEMPORELLE ====================
    
    /**
     * @brief Intègre une vitesse angulaire pour mettre à jour l'orientation
     * Utilisé dans les simulations physiques pour l'intégration temporelle
     * 
     * @param omega_x, omega_y, omega_z: vitesse angulaire (rad/s)
     * @param dt: pas de temps (secondes)
     */
    void integrateAngularVelocity(double omega_x, double omega_y, double omega_z, double dt) {
        // Formule: q(t+dt) = q(t) + 0.5 * dt * omega_quat * q(t)
        // où omega_quat = (0, omega_x, omega_y, omega_z)
        
        Quaternion omega_quat(0.0, omega_x, omega_y, omega_z);
        Quaternion dq = (omega_quat * (*this)) * (0.5 * dt);
        
        w += dq.w;
        x += dq.x;
        y += dq.y;
        z += dq.z;
        
        // Renormalisation pour éviter l'accumulation d'erreurs
        normalize();
    }
    
    // ==================== AFFICHAGE ====================
    
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
        os << "(" << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k)";
        return os;
    }
    
    /**
     * @brief Affichage en format axe-angle
     */
    void printAxisAngle() const {
        double ax, ay, az, angle;
        toAxisAngle(ax, ay, az, angle);
        std::cout << "Axis: (" << ax << ", " << ay << ", " << az << "), "
                  << "Angle: " << angle << " rad (" << angle * 180.0 / M_PI << "°)" << std::endl;
    }
};

#endif // QUATERNION_HPP
