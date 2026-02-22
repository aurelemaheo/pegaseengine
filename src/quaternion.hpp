// @file quaternion.hpp 
// @brief Implementation of a Quaternion class for 3D rotations

#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <cmath>
#include <iostream>
#include <stdexcept>

/**
 * @class Quaternion
 * @brief Quaternion implementation for 3D rotations
 * 
 * A quaternion is denoted q = w + xi + yj + zk
 * where w is the scalar component and (x, y, z) is the vector component
 */
class Quaternion {
private:
    double w, x, y, z;

public:
    // Constructors
    
    /**
     * @brief Default constructor - quaternion identity (no rotation)
     */
    Quaternion() : w(1.0), x(0.0), y(0.0), z(0.0) {}
    
    /**
     * @brief Constructor with specified components
     */
    Quaternion(double w, double x, double y, double z) 
        : w(w), x(x), y(y), z(z) {}
    
    /**
     * @brief Create a quaternion given an axis and an angle (physical representation)
     * @param axis_x, axis_y, axis_z: components of the rotation axis (automatically normalized)
     * @param angle: rotation angle in radians
     */
    static Quaternion fromAxisAngle(double axis_x, double axis_y, double axis_z, double angle) {
        double half_angle = angle * 0.5;
        double sin_half = std::sin(half_angle);
        
        // Aixs normalized
        double norm = std::sqrt(axis_x*axis_x + axis_y*axis_y + axis_z*axis_z);
        if (norm < 1e-10) {
            // Axis null -> quaternion identity
            return Quaternion(1.0, 0.0, 0.0, 0.0);
        } else {
            double w = std::cos(half_angle);
            double x = (axis_x / norm) * sin_half;
            double y = (axis_y / norm) * sin_half;
            double z = (axis_z / norm) * sin_half;
            return Quaternion(w, x, y, z);
        }
    }
    
    // Accessors
    double getW() const { return w; }
    double getX() const { return x; }
    double getY() const { return y; }
    double getZ() const { return z; }
    
    void set(double w_, double x_, double y_, double z_) {
        w = w_; x = x_; y = y_; z = z_;
    }
    
    // Fundamental operations
    
    /**
     * @brief Calculate the magnitude of the quaternion 
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
     * @brief Returns a normalized copy  
     */
    Quaternion normalized() const {
        Quaternion result = *this;
        result.normalize();
        return result;
    }
    
    /**
     * @brief Calculate the conjugate: q* = w - xi - yj - zk
     * Important: for a unit quaternion, q* = q⁻¹
     */
    Quaternion conjugate() const {
        return Quaternion(w, -x, -y, -z);
    }
    
    /**
     * @brief Calculate the inverse: q⁻¹ = q* / |q|²
     */
    Quaternion inverse() const {
        double n2 = w*w + x*x + y*y + z*z;
        if (n2 < 1e-10) {
            throw std::runtime_error("Cannot invert zero quaternion");
        }
        return Quaternion(w/n2, -x/n2, -y/n2, -z/n2);
    }
    
    // Operators
    
    /**
     * @brief Multiplication of quaternions (composed rotations)
     * NOTE: order matters ! q1 * q2 ≠ q2 * q1
     * q1 * q2 means: apply q2 first, then q1
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
     * @brief Adding quaternions
     */
    Quaternion operator+(const Quaternion& q) const {
        return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
    }
    
    /**
     * @brief Subtraction of quaternions
     */
    Quaternion operator-(const Quaternion& q) const {
        return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
    }
    
    /**
     * @brief Multiply by a scalar
     */
    Quaternion operator*(double scalar) const {
        return Quaternion(w*scalar, x*scalar, y*scalar, z*scalar);
    }
    
    /**
     * @brief Divide by a scalar
     */
    Quaternion operator/(double scalar) const {
        if (std::abs(scalar) < 1e-10) {
            throw std::runtime_error("Division by zero");
        }
        return Quaternion(w/scalar, x/scalar, y/scalar, z/scalar);
    }
    
    // Rotation of vectors
    
    /**
     * @brief Applies the rotation depicted by this quaternion to a 3D vector
     * Formula: v' = q * v * q*
     * where v is considered as a pure quaternion (scalar component = 0)
     * 
     * @param vx, vy, vz: vector components to manipulate
     * @param rx, ry, rz: references used for storing result
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
    
    // Conversions
    
    /**
     * @brief Extracts axis and rotation angle 
     * @param axis_x, axis_y, axis_z: references for the normalized axis
     * @param angle: reference for the angle in radians
     */
    void toAxisAngle(double& axis_x, double& axis_y, double& axis_z, double& angle) const {
        double w_clamped = (w < -1.0) ? -1.0 : (w > 1.0 ? 1.0 : w);
        angle = 2.0 * std::acos(w_clamped);
        double sin_half = std::sin(angle * 0.5);
        
        if (std::abs(sin_half) < 1e-10) {
            // No rotation, or full rotation (360°)
            axis_x = 1.0; axis_y = 0.0; axis_z = 0.0;
        } else {
            axis_x = x / sin_half;
            axis_y = y / sin_half;
            axis_z = z / sin_half;
        }
    }
    
    /**
     * @brief Converts to a 3x3 matrix 
     * @param mat: 9 item array (row-major order)
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
     * @brief Converts into Euler angles (yaw, pitch, roll) in radians 
     * Convention: rotation ZYX (yaw, then pitch, then roll)
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
    
    // Interpolation
    
    /**
     * @brief Spherical Linear Interpolation (SLERP)  
     * 
     * @param q1: input quaternion 
     * @param q2: output quaternion 
     * @param t: interpolation parameter [0, 1]
     * @return interpolated quaternion
     */
    static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, double t) {
        // Scalar product to calculate the angle between quaternions
        double dot = q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
        
        // If the dot product is negative, take the shorter path
        Quaternion q2_adjusted = q2;
        if (dot < 0.0) {
            q2_adjusted = q2 * (-1.0);
            dot = -dot;
        }
        
        // If the quaternions are very close, use linear interpolation
        if (dot > 0.9995) {
            Quaternion result = q1 + (q2_adjusted - q1) * t;
            result.normalize();
            return result;
        }
        
        // Calculate SLERP
        double dot_clamped = (dot < -1.0) ? -1.0 : (dot > 1.0 ? 1.0 : dot);
        double theta = std::acos(dot_clamped);
        double sin_theta = std::sin(theta);
        double w1 = std::sin((1.0 - t) * theta) / sin_theta;
        double w2 = std::sin(t * theta) / sin_theta;
        
        return q1 * w1 + q2_adjusted * w2;
    }
    
    // Temporal integration   
    
    /**
     * @brief Integrate angular speed to update orientation
     * Used in physics simulations for temporal integration
     * 
     * @param omega_x, omega_y, omega_z: angular velocity (rad/s)
     * @param dt: time step (seconds)
     */
    void integrateAngularVelocity(double omega_x, double omega_y, double omega_z, double dt) {
        // Formula: q(t+dt) = q(t) + 0.5 * dt * omega_quat * q(t)
        // where omega_quat = (0, omega_x, omega_y, omega_z)
        
        Quaternion omega_quat(0.0, omega_x, omega_y, omega_z);
        Quaternion dq = (omega_quat * (*this)) * (0.5 * dt);
        
        w += dq.w;
        x += dq.x;
        y += dq.y;
        z += dq.z;
        
        // Renormalization to prevent error accumulation
        normalize();
    }
    
    // Display
    
    friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
        os << "(" << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k)";
        return os;
    }
    
    /**
     * @brief Display in axis-angle format
     */
    void printAxisAngle() const {
        double ax, ay, az, angle;
        toAxisAngle(ax, ay, az, angle);
        std::cout << "Axis: (" << ax << ", " << ay << ", " << az << "), "
                  << "Angle: " << angle << " rad (" << angle * 180.0 / M_PI << "°)" << std::endl;
    }
};

#endif // QUATERNION_HPP
